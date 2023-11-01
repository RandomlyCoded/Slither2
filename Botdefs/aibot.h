#ifndef SLITHER_AIBOT_H
#define SLITHER_AIBOT_H

#include "bot.h"

#include <QDebug>

class NeuralNet;

namespace Slither
{

class AiBot : public Bot
{
    Q_OBJECT

public:
    AiBot(Snake *parent);

    void act(qreal dt) override;

    Type type() override { return Bot_Intelligent; }

    void die();

    const qreal age() const { return m_age; }
    const NeuralNet *net() const { return m_net; }

    static NeuralNet *global();

    const int starvation() { return m_starvingTime; }

private:
    NeuralNet *m_net;

    qreal m_age = 0;

    bool maybeStarve();

    int m_starvingTime = 0;
    const int maxStarvation = 500;
};

} // namespace Slither

#endif // SLITHER_AIBOT_H
