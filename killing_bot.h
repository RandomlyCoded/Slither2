#ifndef KILLINGBOT_H
#define KILLINGBOT_H

#include "bot.h"

namespace Slither
{

class StupidBot;

class KillingBot : public Bot
{
public:
    KillingBot(Snake *parent);

    void act(qreal duration) override;

    Type type() override { return Bot_Killing; }

private:
    StupidBot *stupid;
    Snake *m_target = 0;
    bool getTarget();
};

} // namespace Slither

#endif // KILLINGBOT_H
