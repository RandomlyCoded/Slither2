#ifndef SLITHER_BOT_H
#define SLITHER_BOT_H

#include <QRandomGenerator>
#include <QPointF>

//#include "snake.h"
#include "playground.h"

namespace Slither {

class Snake;

class Bot
{
public:
    Bot(Snake *controlled);

    enum ControlFlags {
        circelingSnake,
        intelligent,
        killSnake,
        mediumIntelligence,
        saving,
        stupid,
        tryEat,
    };

    enum Types {
        energyPearl,
        snake,
    };

    void act(qreal duration, ControlFlags moveType = mediumIntelligence);

    void moveSnakeTo(QPointF position);
    void moveSnakeTo(Snake *position);
    void circle(Snake *snake);
    void circle();

private:
    QRandomGenerator *rng = new QRandomGenerator;

    void findNext(Types type);
    Snake *m_snake;

    Snake *nextSnake;
    QPointF nextEnergy;

    double m_r;
    QList<QPointF> m_circlePoints;

    void getCircle(QPointF position);
};

} // namespace Slither

#endif // SLITHER_BOT_H
