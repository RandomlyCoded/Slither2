#ifndef SLITHER_PLAYGROUND_H
#define SLITHER_PLAYGROUND_H

#include "snake.h"

#include <QDebug>
#include <QAbstractListModel>
#include <QColor>
#include <QObject>
#include <QPointF>
#include <QTimer>

namespace Slither {

class Bot;

struct EnergyPearl
{
    qreal amount;
    QPointF position;
    QColor color;

    Q_GADGET
    Q_PROPERTY(qreal amount MEMBER amount CONSTANT FINAL)
    Q_PROPERTY(QPointF position MEMBER position CONSTANT FINAL)
    Q_PROPERTY(QColor color MEMBER color CONSTANT FINAL)
};

struct Chunk
{
    QPoint pos;
    QList<Slither::EnergyPearl> pearls;
};

template<class T>
class DataListModel : public QAbstractListModel
{
public:
    enum Roles {
        DataRole = Qt::UserRole + 1
    };

    using QAbstractListModel::QAbstractListModel;

    void reset(QList<T> rows = {});
    void add(T rows = {});
    void remove(QModelIndex at);
    T &at(int index);

public: // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role = DataRole) const override;
    bool setData(const QModelIndex &index, const QVariant &data, int role = DataRole) override;
    QHash<int, QByteArray> roleNames() const override { return {{DataRole, "data"}}; }

protected:
    QList<T> m_rows;
};

template <class T>
T &DataListModel<T>::at(int index)
{
    return m_rows[index];
}
template<class T>
void DataListModel<T>::reset(QList<T> rows)
{
    beginResetModel();
    std::swap(m_rows, rows);
    endResetModel();
}

template<class T>
void DataListModel<T>::add(T t)
{
    m_rows.append(t);
}

template<class T>
void DataListModel<T>::remove(QModelIndex at)
{
    m_rows.removeAt(at.row());
}

template<class T>
int DataListModel<T>::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_rows.count();
}

template<class T>
QVariant DataListModel<T>::data(const QModelIndex &index, int role) const
{
    if (checkIndex(index) && role == DataRole)
        return QVariant::fromValue(m_rows[index.row()]);

    return {};
}

template<class T>
bool DataListModel<T>::setData(const QModelIndex &index, const QVariant &data, int role)
{
    if (checkIndex(index) && role == DataRole && data.canConvert<T>()) {
//        qInfo("CHANGE");
        m_rows[index.row()] = data.value<T>();
        emit dataChanged(index, index, {role});
        return true;
    }

    return false;
}

class EnergyPearlListModel : public DataListModel<EnergyPearl>
{
    Q_OBJECT;

public:
    using DataListModel::DataListModel;
    EnergyPearlListModel(EnergyPearlListModel &other) { m_rows = other.m_rows; }
};

class Playground : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal size READ size NOTIFY sizeChanged FINAL)
    Q_PROPERTY(Slither::EnergyPearlListModel *energyPearls READ energyPearls NOTIFY energyPearlsChanged FINAL)
    Q_PROPERTY(QList<Slither::Snake *> snakes READ snakes NOTIFY snakesChanged FINAL)

    Q_PROPERTY(int snakeCount READ snakeCount NOTIFY snakesChanged FINAL)
    Q_PROPERTY(int pearlAmount READ pearlAmount NOTIFY energyPearlsChanged FINAL)
    Q_PROPERTY(int totalSnakesSize READ totalSnakesSize NOTIFY totalSnakesSizeChanged FINAL)

    Q_PROPERTY(bool masshacks READ masshacks NOTIFY masshacksChanged FINAL)

public:
    Playground(QObject *parent = nullptr);
    qreal size() const noexcept { return m_size; }
    EnergyPearlListModel *energyPearls() const { return m_energyPearls; }
    QList<Snake*> snakes() { return m_snakes; };

    bool checkCrash(Snake *toCheck);

    int snakeCount() const { return m_snakes.count(); }

    int pearlAmount() const { return m_energyPearls->rowCount(); }

    int totalSnakesSize() const;

    constexpr const static qreal boostSpeed  = 10;
    constexpr const static qreal normalSpeed =  5;
    bool checkBounds(Snake *snake) const;

    bool masshacks() const { return m_masshacksActive; }

public slots:
    void tp()
    {
        for(int i = i; i < m_energyPearls->rowCount(); i++)
            m_energyPearls->at(i).position = QPointF(0, 0);
    }
    void switch_masshacks() { m_masshacksActive ^= 1; }
    void initialize(qreal size);

    bool checkBounds(QPointF position) const;
    qreal consumeNearbyPearls(QPointF position, const Snake *eater = nullptr);

    void addSnake(Snake *snake);
    void killSnake(Snake *snake);
    void spawnSnake();

    EnergyPearl spawnPearl() const;
    EnergyPearl addPearl(QPointF position = {0, 0}, qreal amount = 1, QColor color = "#7f7f7f",
                         bool autoadd = false) const;

    QColor color(int H) { return QColor::fromHsv(H, 100, 100); }

    void save();
    void load();
    void load(QString filename);

    void moveSnakes(qreal dt);

    int indexOfSnake(Snake *snake) { return m_snakes.indexOf(snake); }

    QList<QPointF> zoomOut(QList<QPointF> positions, int zoom)
    { QList<QPointF> ret; for(const auto &pos: positions) ret += (pos * zoom); return ret; }

signals:
    void sizeChanged(qreal size);
    void snakesChanged(QList<Snake *> snakes);

    void colorChanged();

    void energyPearlsChanged();

    void totalSnakesSizeChanged();

    void masshacksChanged();

private:
    QTimer *m_newSnakeTimer = new QTimer{this};
    QTimer *m_energyTimer = new QTimer{this};

    qreal m_size = 200;
    QList<Snake*> m_snakes;
    EnergyPearlListModel *const m_energyPearls = new EnergyPearlListModel{this};

    int m_maximumEnergy;

    void energyBoost();
    int m_pearlAmount;
    int m_snakeCount;
    bool m_masshacksActive = 0;
};


} // namespace Slither

Q_DECLARE_METATYPE(QList<Slither::EnergyPearl>)
Q_DECLARE_METATYPE(QList<Slither::Snake>)

#endif // SLITHER_PLAYGROUND_H
