#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>

#include "gamepresenter.h"
#include "exceptions.h"
#include "gameinitproxy.h"


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

    qmlRegisterType(QUrl("qrc:///Application/AppMain/IntroPane.qml"), "AppCore", 1, 0, "IntroPane");
    qmlRegisterType(QUrl("qrc:///Application/AppMain/HelpPane.qml"), "AppCore", 1, 0, "HelpPane");
    qmlRegisterType(QUrl("qrc:///Application/AppMain/MainPane.qml"), "AppCore", 1, 0, "MainPane");
    qmlRegisterType(QUrl("qrc:///Application/AppMain/DataEntryPane.qml"), "AppCore", 1, 0, "DataEntryPane");

    qmlRegisterType(QUrl("qrc:///Application/Controls/AppButton.qml"), "Controls", 1, 0, "AppButton");
    qmlRegisterType(QUrl("qrc:///Application/Controls/AppSwitch.qml"), "Controls", 1, 0, "AppSwitch");
    qmlRegisterType(QUrl("qrc:///Application/Controls/AppRadioButton.qml"), "Controls", 1, 0, "AppRadioButton");
    qmlRegisterType(QUrl("qrc:///Application/Controls/Dropdown.qml"), "Controls", 1, 0, "Dropdown");
    qmlRegisterType(QUrl("qrc:///Application/Controls/MixedWordPieces.qml"), "Controls", 1, 0, "MixedWordPieces");
    qmlRegisterType(QUrl("qrc:///Application/Controls/WordPiecesInput.qml"), "Controls", 1, 0, "WordPiecesInput");

    qmlRegisterType(QUrl("qrc:///Application/Dialogs/ErrorPane.qml"), "AppDialogs", 1, 0, "ErrorPane");
//    qmlRegisterType(QUrl("qrc:///Application/Dialogs/ErrorDialog.qml"), "AppDialogs", 1, 0, "ErrorDialog"); // not required in QML but only in main.cpp
    qmlRegisterType(QUrl("qrc:///Application/Dialogs/PromptDiscardPane.qml"), "AppDialogs", 1, 0, "PromptDiscardPane");
    qmlRegisterType(QUrl("qrc:///Application/Dialogs/PromptSaveExitPane.qml"), "AppDialogs", 1, 0, "PromptSaveExitPane");

    qmlRegisterType(QUrl("qrc:///Application/OtherWidgets/StatusBox.qml"), "StatusBoxes", 1, 0, "StatusBox");
    qmlRegisterType(QUrl("qrc:///Application/OtherWidgets/StatisticsBox.qml"), "StatusBoxes", 1, 0, "StatisticsBox");

    qmlRegisterSingletonType(QUrl("qrc:///Application/Utilities/GameStrings.qml"), "GameUtils", 1, 0, "GameStrings");
    qmlRegisterSingletonType(QUrl("qrc:///Application/Utilities/Styles.qml"), "GameUtils", 1, 0, "Styles");
    qmlRegisterSingletonType(QUrl("qrc:///Application/Utilities/Animations.qml"), "Animations", 1, 0, "Animations");
}
