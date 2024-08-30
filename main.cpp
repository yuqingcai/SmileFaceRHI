#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // qputenv("QSG_RHI_BACKEND", "opengl");

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("SmileFaceRHI", "Main");
    // engine.loadFromModule("SmileFaceRHI", "ProjectManager");


    return app.exec();
}
