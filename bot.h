#ifndef SLITHER_BOT_BASE_H
#define SLITHER_BOT_BASE_H

#ifndef SLITHER_BOT_H
#define SLITHER_BOT_H

#include <QRandomGenerator>
#include <QPointF>
#include <QObject>

#define MOVING_FUNCS private

namespace Slither {

class Snake;
class Playground;

namespace {

bool is_inrange(qreal dist, qreal value, qreal range);

}

class Bot : public QObject
{
    void remove();

public:
    ~Bot() { }
    Bot(Snake *controlled);

    virtual void act(qreal duration) = 0;

protected:
    Playground *playground() const;
    const QPointF position() const;
    Snake *m_snake;
};

} // namespace Slither

#endif // SLITHER_BOT_H

#endif // SLITHER_BOT_BASE_H
