#ifndef AIBOT_NET2_H

#define AIBOT_NET2_H

#include "netBase.h"

namespace Slither
{

class AiBot;


class Net2 : public NetBase
{
public:
    Net2(AiBot *s);

    void act(qreal dt) override;

    const Type type() const override { return Type::NetA; }

    static void mutate(NeuralNet *n);
    static NeuralNet *globalNet();

private:
    AiBot *m_bot;
};

} // namespace Slither

#endif // AIBOT_NET2_H
