#include "playground.h"
#include "snake.h"
#include "bot.h"
#include "leaderboard.h"

#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>

class Application : public QGuiApplication
{
public:
    using QGuiApplication::QGuiApplication;
    int run();
};

int Application::run()
{
    setWindowIcon(QIcon("qrc:/icon/Slither2.ico"));
    qmlRegisterUncreatableType<Slither::EnergyPearl>("Slither", 1, 0, "energyPerl", "This is a value type");
    qmlRegisterType<Slither::Playground>("Slither", 1, 0, "Playground");
    qmlRegisterType<Slither::Snake>("Slither", 1, 0, "Snake");
    qmlRegisterType<Slither::Leaderboard>("Slither", 1, 0, "Leaderboard");

    QQmlApplicationEngine engine;

    const QUrl url{"qrc:/main.qml"};

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
