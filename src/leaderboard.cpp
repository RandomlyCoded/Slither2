#include "leaderboard.h"
#include "snake.h"

#include <QDebug>

namespace Slither {

Leaderboard::Leaderboard(Playground *parent)
    : QObject(parent)
    , m_playground(parent)
{
    init();
}

QList<Snake *> Leaderboard::leaderboard()
{
    reload();
    return m_leaderSnakes;
}

void Leaderboard::reload()
{
    setLeaderSnakes();
}

void Leaderboard::setPlayground(Playground *newPlayground)
{
    m_playground = newPlayground;
}

void Leaderboard::init()
{
    m_leaderSnakes = {};
}

bool compSnakes(Snake *a, Snake *b)
{ return *a > *b; } // if we compare using '<' we get the list in the wrong order

void Leaderboard::setLeaderSnakes()
{
    QList<Snake *> possibleSnakes = m_playground->snakes();

    std::sort(possibleSnakes.begin(), possibleSnakes.end(), compSnakes); // sort the snakes using compSnakes.

    m_leaderSnakes = {};
    for(int i = 0; i < qMin(possibleSnakes.size(), 10); i++) {
        m_leaderSnakes.append(possibleSnakes[i]);
    }

    emit leaderboardChanged();
}

} // namespace Slither

#include "moc_leaderboard.cpp"
