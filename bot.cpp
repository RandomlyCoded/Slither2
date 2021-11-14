#include "bot.h"

#include <QRandomGenerator>
#include <QDebug>

namespace Slither {

Bot::Bot(Snake *controlled)
{
//    qInfo() << "Snake" << controlled << "got Bot" << this;
    m_snake = controlled;
}

void Bot::act(qreal duration, ControlFlags moveType)
{
    switch(moveType) {
    case mediumIntelligence: {
        act(duration, saving);
//        switch(rng->bounded(1)) {
//        case 0: act(duration, stupid); break;
//        case 1: act(duration, intelligent); break;
//        }
    } return;
    case tryEat: {
//        findNext(energyPearl); // needs to much time, Program chrashes
        moveSnakeTo(QPointF{nextEnergy});
    } return;
    case saving:
        circle();
        return;
    case circelingSnake: {
        findNext(snake);
        circle(nextSnake);
    } return;
    case killSnake: {
//        qInfo() << "I am a bot:" << this << ". Want to kill a snake but I don't know how... :.("; // BESTE
        findNext(snake);

        circle(nextSnake);
    } return;
    case stupid: {
        qreal x = rng->bounded(2.) - 1;
        qreal y = rng->bounded(2.) - 1;
        auto dest = m_snake->position() + QPointF{x, y};
        qInfo() << "moving stupid" << dest << "(" << x << ", " << y << ")";
        m_snake->setDestination(dest);
        m_snake->move(duration);
    } return;
    case intelligent: {
        qInfo() << "try to be intelligent(with an IQ of ~0...)";
        m_snake->move(duration); return;
    }
    }
}

void Bot::moveSnakeTo(QPointF position)
{

}

void Bot::moveSnakeTo(Snake *position)
{
    moveSnakeTo(position->position());
}

void Bot::circle(Snake *snake)
{
    circle();
}

void Bot::circle()
{
    if(m_circlePoints.isEmpty())
        getCircle(m_snake->position());
    QPointF moveTo = m_circlePoints.first();
    moveSnakeTo(moveTo);
    m_circlePoints.removeFirst();
    m_circlePoints.append(moveTo);
}

void Bot::getCircle(QPointF position)
{
    m_r = m_snake->lenght() / M_PI / 2;

    for(int i = 0; i < 360; i++)
        m_circlePoints.append(position + QPointF { m_r * cos(i), m_r * sin(i) });
}

void Bot::findNext(Types type)
{
    if(type == snake) {
        if(m_snake->playground()->snakes().isEmpty())
            return;
        for(const auto sn: m_snake->playground()->snakes()) {
            qInfo() << m_snake->playground()->snakes().count();
            if(nextSnake == nullptr)
                nextSnake = sn;

            if(QVector2D{sn->position()}.distanceToPoint(QVector2D{m_snake->position()}) <
                    QVector2D{nextSnake->position()}.distanceToPoint(QVector2D{m_snake->position()}))
                nextSnake = sn;
        }
    }

    else if(type == energyPearl)
        nextEnergy = QPointF{rng->generateDouble(), rng->generateDouble()};
}

} // namespace Slither
