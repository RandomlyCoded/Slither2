#ifndef SLITHER_PLAYGROUND_H
#define SLITHER_PLAYGROUND_H

#include <QAbstractListModel>
#include <QColor>
#include <QObject>
#include <QPointF>
#include <QTimer>

namespace Slither {

class Snake;

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

template<class T>
class DataListModel : public QAbstractListModel
{
public:
    enum Roles {
        DataRole = Qt::UserRole + 1
    };

    using QAbstractListModel::QAbstractListModel;

    void reset(QList<T> rows = {});

public: // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role = DataRole) const override;
    bool setData(const QModelIndex &index, const QVariant &data, int role = DataRole) override;
    QHash<int, QByteArray> roleNames() const override { return {{DataRole, "data"}}; }

private:
    QList<T> m_rows;
};

template<class T>
void DataListModel<T>::reset(QList<T> rows)
{
    beginResetModel();
    std::swap(m_rows, rows);
    endResetModel();
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
};

class Playground : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal size READ size NOTIFY sizeChanged FINAL)
    Q_PROPERTY(Slither::EnergyPearlListModel *energyPearls READ energyPearls CONSTANT FINAL)
    Q_PROPERTY(QList<Slither::Snake *> snakes READ snakes NOTIFY snakesChanged FINAL)

public:
    qreal size() const { return m_size; }
    EnergyPearlListModel *energyPearls() const { return m_energyPearls; }
    QList<Snake *> snakes() const { return m_snakes; }
    void checkCrash(Snake *toCheck);

public slots:
    void initialize(qreal size);

    bool checkBounds(QPointF position) const;
    qreal consumeNearbyPearls(QPointF position);

    void addSnake(Slither::Snake *snake);
    void killSnake(Slither::Snake *snake);
    void spawnSnake();

signals:
    void sizeChanged(qreal size);
    void snakesChanged(QList<Slither::Snake *> snakes);

private:

    EnergyPearl spawnPearl() const;

    QTimer *m_newSnakeTimer;

    qreal m_size = 1000;
    QList<Slither::Snake *> m_snakes;
    EnergyPearlListModel *const m_energyPearls = new EnergyPearlListModel{this};
};

} // namespace Slither

Q_DECLARE_METATYPE(QList<Slither::EnergyPearl>)

#endif // SLITHER_PLAYGROUND_H
