#ifndef SLITHER_STUPIDBOT_H
#define SLITHER_STUPIDBOT_H

#include "bot.h"

namespace Slither
{

class StupidBot : public Bot
{
public:
    StupidBot(Snake *parent);

    void act(qreal duration) override;
private:
    bool wouldChrash(qreal dt);
    void changeDestination();
};

} // namespace Slither

#endif // SLITHER_STUPIDBOT_H
