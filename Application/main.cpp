#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>

#include "gamepresenter.h"
#include "../Common/exceptions.h"


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    qmlRegisterType<GamePresenter>("GameManagers",1,0,"GamePresenter");

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    try
    {
        engine.load(QUrl(QLatin1String("qrc:/main.qml")));
        if (engine.rootObjects().isEmpty())
            return -1;

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
