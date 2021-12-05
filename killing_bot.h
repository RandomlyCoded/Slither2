#ifndef KILLINGBOT_H
#define KILLINGBOT_H

#include "bot.h"

namespace Slither
{

class KillingBot : public Bot
{
public:
    KillingBot(Snake *parent);

    void act(qreal duration) override;

private:
    Snake *m_target = 0;
    bool getTarget();
};

} // namespace Slither

#endif // KILLINGBOT_H
