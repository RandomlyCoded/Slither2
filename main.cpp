#include "playground.h"
#include "snake.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>

class Application : public QGuiApplication
{
public:
    using QGuiApplication::QGuiApplication;
    int run();
};

int Application::run()
{
    qmlRegisterUncreatableType<Slither::EnergyPearl>("Slither", 1, 0, "EnergyPerl", "This is a value type");
    qmlRegisterType<Slither::Playground>("Slither", 1, 0, "Playground");
    qmlRegisterType<Slither::Snake>("Slither", 1, 0, "Snake");

    QQmlApplicationEngine engine;

    const QUrl url{QLatin1String{"qrc:/main.qml"}};

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     this, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            exit(EXIT_FAILURE);
    }, Qt::QueuedConnection);

    engine.load(url);
    return exec();
}

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    return Application{argc, argv}.run();
}
