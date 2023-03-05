#ifndef EATINGBOT_H
#define EATINGBOT_H

#include "bot.h"

namespace Slither
{

class EatingBot : public Bot
{
public:
    EatingBot(Snake *parent);
    Type type() override { return Bot_Eating; }

    void act(qreal dt) override;
};

} // namespace Slither

#endif // EATINGBOT_H
