#ifndef SLITHER_AIBOT_H
#define SLITHER_AIBOT_H

#include "NeuralNets/netBase.h"

#include "bot.h"

#include <QDebug>

class NeuralNet;

namespace Slither
{

class Net1;
class Net2;

class AiBot : public Bot
{
    Q_OBJECT

public:
    AiBot(Snake *parent);

    void act(qreal dt) override;

    Type type() override { return Bot_Intelligent; }

    using NetType = NetBase::Type;

    void die();

    const qreal age() const { return m_age; }
    const NeuralNet *net() const { return m_net; }

private:
    NeuralNet *m_net;

    qreal m_age = 0;

    friend Net2;
};

} // namespace Slither

#endif // SLITHER_AIBOT_H
