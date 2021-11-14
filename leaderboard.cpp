#include "leaderboard.h"

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

template<class ContainerType>
ContainerType rangeOf(ContainerType container, int amount)
{
    ContainerType ret = {};
    for(int i = 0; i < amount; i++) {
        if(container[i])
            ret += container[i];
        else {
            qWarning() << "index" << i << "of container is not valid";
            return ret;
        }
    }
    return ret;
}

void Leaderboard::setLeaderSnakes()
{
    std::sort(m_possibleSnakes.begin(), m_possibleSnakes.end());

    m_leaderSnakes = rangeOf<QList<Snake*>>(m_possibleSnakes, 10);

    emit leaderboardChanged();
}



} // namespace Slither

 #include "moc_leaderboard.cpp"
