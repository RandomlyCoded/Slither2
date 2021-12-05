#ifndef SLITHER_LEADERBOARD_H
#define SLITHER_LEADERBOARD_H

#include <QObject>

#include "snake.h"

namespace Slither {

class Snake;

class Leaderboard : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QList<Data> leaderboard READ leaderboard NOTIFY leaderboardChanged FINAL)
    Q_PROPERTY(Slither::Playground *playground MEMBER m_playground WRITE setPlayground FINAL)

public:
    struct Data;

    Leaderboard(QObject *parent = nullptr);
    Leaderboard(QList<Snake*> snakes);

    const QList<Data> &leaderboard() const { return m_leaderSnakes; }

    void setPlayground(Slither::Playground *newPlayground) { m_playground = newPlayground; }

signals:
    void leaderboardChanged();

private:
    QList<Snake*> m_possibleSnakes;
    QList<Data> m_leaderSnakes;

    void init();
    void setLeaderSnakes();
    QPointer<Slither::Playground> m_playground;
};

struct Leaderboard::Data
{
    int size;
    int index;
    QString name;

    Q_GADGET
    Q_PROPERTY(int size MEMBER size CONSTANT FINAL)
    Q_PROPERTY(int index MEMBER index CONSTANT FINAL)
    Q_PROPERTY(QString snakeName MEMBER name CONSTANT FINAL)
};

} // namespace Slither

#endif // SLITHER_LEADERBOARD_H
