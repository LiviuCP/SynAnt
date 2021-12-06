#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>

#include "exceptions.h"
#include "gameinitproxy.h"

extern void registerDataTypesForQML();

int main(int argc, char* argv[])
{
    registerDataTypesForQML();

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    try
    {
        GameInitProxy gameInitProxy;

        gameInitProxy.setEnvironment(app.applicationDirPath());
        engine.load(QUrl(QLatin1String("qrc:/Application/main.qml")));

        if (engine.rootObjects().isEmpty())
        {
            return -1;
        }

        return app.exec();
    }
    catch (const GameException& exception)
    {
        QQmlComponent errorComponent(&engine, QUrl(QLatin1String("qrc:/Application/Dialogs/ErrorDialog.qml")));

        QObject* errorComponentObject{errorComponent.create()};
        QObject* errorDialogObject{errorComponentObject->findChild<QObject*>("errorDialog")};

        if (errorDialogObject)
        {
            errorDialogObject->setProperty("text", exception.getDescription());
        }

        return app.exec();
    }
}
