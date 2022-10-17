#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QJSEngine>
#include <QtGlobal>
#include "../qt-toolkit/App.h"
#include "../qt-toolkit/Engine.h"
#include "../qt-toolkit/FileSystem.h"
#include "../qt-toolkit/Folder.h"
#include "../qt-toolkit/SyntaxHighlighter.h"
#include "../qt-toolkit/System.h"

int main(int argc, char *argv[])
{
    QString newSelectors = QString("qt") + QString::number(QT_VERSION_MAJOR)
                        + QString(",qt") + QString::number(QT_VERSION_MAJOR)
                          + QString(".") + QString::number(QT_VERSION_MINOR)
                        + QString(",qt") + QString::number(QT_VERSION_MAJOR)
                          + QString(".") + QString::number(QT_VERSION_MINOR)
                          + QString(".") + QString::number(QT_VERSION_PATCH);

    qputenv("QT_FILE_SELECTORS",newSelectors.toUtf8());

    qmlRegisterSingletonType<App>("qmlonline", 1, 0, "App", [](QQmlEngine*,QJSEngine*) -> QObject* { return new App(); } );
    qmlRegisterSingletonType<Engine>("qmlonline", 1, 0, "Engine", [](QQmlEngine*,QJSEngine*) -> QObject* { return new Engine(); } );
    qmlRegisterSingletonType<FileSystem>("qmlonline", 1, 0, "FileSystem", [](QQmlEngine*,QJSEngine*) -> QObject* { return new FileSystem(); } );
    qmlRegisterSingletonType<System>("qmlonline", 1, 0, "System", [](QQmlEngine*,QJSEngine*) -> QObject* { return new System(); } );

    qmlRegisterType<Folder>("qmlonline", 1, 0, "Folder");
    qmlRegisterType<SyntaxHighlighter>("qmlonline", 1, 0, "SyntaxHighlighter");

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/qml/MyApp.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}