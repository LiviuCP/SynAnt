#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>

#include "gamepresenter.h"
#include "../Common/exceptions.h"
#include "../Common/gameinitproxy.h"


void registerDataTypesForQML();

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    GameInitProxy gameInitProxy;

    registerDataTypesForQML();

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    try
    {
        gameInitProxy.setDataSource(app.applicationDirPath());
        engine.load(QUrl(QLatin1String("qrc:/main.qml")));

        if (engine.rootObjects().isEmpty())
        {
            return -1;
        }

        return app.exec();
    }
    catch (const GameException& exception)
    {
        QQmlComponent errorComponent(&engine, QUrl(QLatin1String("qrc:/ErrorDialog.qml")));
        QObject *errorComponentObject = errorComponent.create();

        QObject *errorDialogObject = errorComponentObject->findChild<QObject*>("errorDialog");

        if (errorDialogObject)
        {
            errorDialogObject->setProperty("text", exception.getDescription());
        }

        return app.exec();
    }
}

void registerDataTypesForQML()
{
    qmlRegisterType<GamePresenter>("GameManagers", 1, 0, "GamePresenter");

    qmlRegisterSingletonType(QUrl("qrc:///GameStrings.qml"), "GameUtils", 1, 0, "GameStrings");
    qmlRegisterSingletonType(QUrl("qrc:///Styles.qml"), "GameUtils", 1, 0, "Styles");
}
