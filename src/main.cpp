#include "../Botdefs/aibot.h"
#include "../Botdefs/bot.h"
#include "../Botdefs/neuralnet.h"

#include "playground.h"
#include "snake.h"
#include "leaderboard.h"
#include "chunk.h"

#include <QDateTime>
#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>

class Application : public QGuiApplication
{
    Q_PROPERTY(QString startmap READ startmap CONSTANT FINAL)

public:
    using QGuiApplication::QGuiApplication;

    int run();
    const QString startmap() const
    {
        if(arguments().length() <= 1)
            return "";
        return arguments()[1];
    }
};

int Application::run()
{
    setAttribute(Qt::AA_CompressHighFrequencyEvents);

    qmlRegisterUncreatableType<Slither::EnergyPearl>("Slither", 1, 0, "energyPerl", "This is a value type");

    qmlRegisterType<Slither::Chunk>("Slither", 1, 0, "Chunk");
    qmlRegisterType<Slither::Snake>("Slither", 1, 0, "Snake");

    Slither::Playground *playground = new Slither::Playground(this);
    qmlRegisterSingletonInstance<Slither::Playground>("Slither", 1, 0, "Playground", playground);

    Slither::Leaderboard *leaderboard = new Slither::Leaderboard(playground);
    qmlRegisterSingletonInstance<Slither::Leaderboard>("Slither", 1, 0, "Leaderboard", leaderboard);

    QQmlApplicationEngine engine;

    const QUrl url{"qrc:/main.qml"};

    engine.load(url);

    connect(this, &QGuiApplication::aboutToQuit, [=] { Slither::AiBot::global()->save(QDateTime::currentDateTime().toString("dd.MM.yyyy+hh.mm.ss") + ".neuralnet"); });
    return exec();
}

int main(int argc, char *argv[])
{
    Application::setAttribute(Qt::AA_UseOpenGLES);
    return Application{argc, argv}.run();
}


