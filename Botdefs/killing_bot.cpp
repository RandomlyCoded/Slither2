#include "killing_bot.h"

#include "snake.h"
#include "stupid_bot.h"

namespace Slither
{

KillingBot::KillingBot(Snake *controlled)
    : Bot(controlled)
    , stupid(new StupidBot(controlled))
{}

void KillingBot::act(qreal duration)
{
    if(!m_target)
        if(!getTarget()) {
            stupid->act(duration);
            return;
        }

    m_snake->setDestination(m_target->position());
}

bool KillingBot::getTarget()
{
    auto snakes = playground()->snakes();
    snakes.removeAll(m_snake);
    QRandomGenerator rng;
    if(snakes.isEmpty())
        return 0;
    if(snakes.count() == 1) {
        m_target = snakes[0];
        return 0;
    }
    m_target = snakes[rng.bounded(snakes.count()) - 1];
    return 1;
}

} // namespace Slither
