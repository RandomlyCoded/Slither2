#ifndef SLITHER_LEADERBOARD_H
#define SLITHER_LEADERBOARD_H

#include <QObject>

#include "snake.h"

namespace Slither {

class Snake;

class Leaderboard : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QList<Snake*> leaderboard READ leaderboard NOTIFY leaderboardChanged FINAL)

public:
    Leaderboard(QObject *parent = nullptr);
    Leaderboard(QList<Snake*> snakes);

    const QList<Snake *> &leaderboard() const { return m_leaderSnakes; }

signals:
    void leaderboardChanged();

private:
    QList<Snake*> m_possibleSnakes;
    QList<Snake*> m_leaderSnakes;

    void init();
    void setLeaderSnakes();
};

} // namespace Slither

#endif // SLITHER_LEADERBOARD_H
