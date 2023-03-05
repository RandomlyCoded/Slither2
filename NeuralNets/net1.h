#ifndef AIBOT_NET1_H

#define AIBOT_NET1_H

#include "netBase.h"

namespace Slither
{

class AiBot;


class Net1 : public NetBase
{
public:
    Net1(AiBot *s);

    void act(qreal dt) override;

    const Type type() const override { return Type::NetA; }

    static void mutate(NeuralNet *n);

    static NeuralNet *globalNet();

private:
    AiBot *m_bot;
};

} // namespace Slither

#endif // AIBOT_NET1_H
