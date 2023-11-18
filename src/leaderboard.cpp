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

/*    qInfo() << "before std::sort:";
    for(auto &sn: possibleSnakes)
        qInfo() << "\t" << sn << sn->lengthInfo();
    qInfo() << "\n";
*/ // debug stuff, actually not needed anymore

    std::sort(possibleSnakes.begin(), possibleSnakes.end(), compSnakes); // sort the snakes using compSnakes.

/*
    qInfo() << "after std::sort:";
    for(auto &sn: possibleSnakes)
        qInfo() << "\t" << sn << sn->lengthInfo();
    qInfo() << "\n";
*/ // debug stuff.

    m_leaderSnakes = {};
    for(int i = 0; i < qMin(possibleSnakes.size(), 10); i++) {
        m_leaderSnakes.append(possibleSnakes[i]);
//        qInfo().nospace() << "SNAKE #" << i << ": " << m_leaderSnakes[i]->lengthInfo() << " " << m_leaderSnakes[i]->name();
    }

    emit leaderboardChanged();
}

} // namespace Slither

#include "moc_leaderboard.cpp"
