#include "playground.h"
#include "snake.h"
#include "bot.h"
#include "leaderboard.h"

#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>

class Application : public QGuiApplication
{
    Q_PROPERTY(QString startmap READ startmap CONSTANT FINAL)
    int m_argc;
    char **m_argv;

public:
    Application(int argc, char **argv)
        : QGuiApplication(argc, argv)
        , m_argc(argc)
        , m_argv(argv)
    {}
    int run();
    const QString startmap() const
    {
        if(m_argc <= 1)
            return "";
        return m_argv[1];
    }
};

int Application::run() // FIXME: nach dem 3.mal spawnen stürzt das Program ab
{
    setWindowIcon(QIcon("qrc:/icon/Slither2.ico"));
    qmlRegisterUncreatableType<Slither::EnergyPearl>("Slither", 1, 0, "energyPerl", "This is a value type");
    qmlRegisterUncreatableType<Slither::Leaderboard::Data>("Slither", 1, 0, "leaderbordData", "This is a value type");
    qmlRegisterType<Slither::Playground>("Slither", 1, 0, "Playground");
    qmlRegisterType<Slither::Snake>("Slither", 1, 0, "Snake");
    qmlRegisterType<Slither::Leaderboard>("Slither", 1, 0, "Leaderboard");
    qmlRegisterSingletonInstance<Application>("Slither", 1, 0, "APP", this);

    QQmlApplicationEngine engine;

    const QUrl url{"qrc:/main.qml"};

    engine.load(url);
    return exec();
}

int main(int argc, char *argv[])
{
    return Application{argc, argv}.run();
}


