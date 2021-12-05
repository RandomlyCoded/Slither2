#include "leaderboard.h"

#include "bot.h"

#include <QDebug>

namespace Slither {

Leaderboard::Leaderboard(QObject *parent)
    : m_possibleSnakes({})
{
    init();
}

Leaderboard::Leaderboard(QList<Snake*> snakes)
    : m_possibleSnakes(snakes)
{
    init();
}

void Leaderboard::init()
{
    m_leaderSnakes.reserve(10);

    setLeaderSnakes();
}

void Leaderboard::setLeaderSnakes()
{
    std::sort(m_possibleSnakes.begin(), m_possibleSnakes.end());
    m_leaderSnakes = {};
    for(int i = 0; i < qMin(m_leaderSnakes.size(), 10); i++) {
        Data d;
        d.size = m_possibleSnakes[i]->size();
        d.index = i;
        d.name = m_possibleSnakes[i]->name();
        m_leaderSnakes.append(d);
    }

    emit leaderboardChanged();
}



} // namespace Slither

#include "moc_leaderboard.cpp"
