#ifndef SLITHER_BOT_BASE_H
#define SLITHER_BOT_BASE_H

#ifndef SLITHER_BOT_H
#define SLITHER_BOT_H

#include <QRandomGenerator>
#include <QPointF>
#include <QObject>

#define MOVING_FUNCS private

namespace Slither {

struct EnergyPearl;
class Snake;
class Playground;

namespace {

bool is_inrange(qreal dist, qreal value, qreal range);

}

class Bot : public QObject
{
    Q_OBJECT

public:
    enum Type : uchar
    {
        Bot_Eating,      // EatingBot
        Bot_NoBot,       // FollowMouseBot
        Bot_Killing,     // KillingBot
        Bot_Stupid,      // StupidBot
        Bot_Intelligent, // AIBot
    };

    Q_ENUM(Type)

    Bot(Snake *controlled);

    /// DO NOT ENTER m_snake->move(duration)
    virtual void act(qreal duration) = 0;
    virtual Type type() = 0;
    QPointF position() const;
    Snake *snake();

protected:
    QPointF destination() const;
    Playground *playground() const;
    Snake *m_snake;

    Slither::EnergyPearl findNextFood();
    QPointF findNextSnakeSegment();
    QPointF findBorder();
};

} // namespace Slither

#endif // SLITHER_BOT_H

#endif // SLITHER_BOT_BASE_H
