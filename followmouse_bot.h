#ifndef SLITHER_FOLLOWMOUSE_BOT_H
#define SLITHER_FOLLOWMOUSE_BOT_H

#include "bot.h"

namespace Slither {

class FollowMouseBot : public Bot
{
public:
    FollowMouseBot(Snake *controlled);

    void act(qreal dt) override;
};

} // namespace Slither

#endif // SLITHER_FOLLOWMOUSE_BOT_H
