#ifndef SLITHER_LEADERBOARD_H
#define SLITHER_LEADERBOARD_H

#include <QObject>
#include <QPointer>

#include "playground.h"
#include "snake.h"

namespace Slither {

class Leaderboard : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QList<Slither::Snake *> leaderboard READ leaderboard NOTIFY leaderboardChanged FINAL)
    Q_PROPERTY(Slither::Playground *playground MEMBER m_playground WRITE setPlayground FINAL)

public:
    Leaderboard(Playground *parent = nullptr);

    QList<Snake *> leaderboard();

    void reload();
    void setPlayground(Slither::Playground *newPlayground);

signals:
    void leaderboardChanged();

private:
    QList<Snake *> m_leaderSnakes;

    void setLeaderSnakes();
    void init();
    QPointer<Playground> m_playground;
};

} // namespace Slither

#endif // SLITHER_LEADERBOARD_H
