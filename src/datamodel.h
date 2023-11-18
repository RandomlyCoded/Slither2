#ifndef ENERGYPEARL_H
#define ENERGYPEARL_H

#include <QAbstractListModel>
#include <QColor>
#include <QPointF>

namespace Slither
{

template<class T>
class DataListModel : public QAbstractListModel
{
public:
    enum Roles {
        DataRole = Qt::UserRole + 1
    };

    using QAbstractListModel::QAbstractListModel;

    typename QList<T>::Iterator begin() { return m_rows.begin(); }
    typename QList<T>::Iterator end() { return m_rows.end(); }

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
        m_rows[index.row()] = data.value<T>();
        emit dataChanged(index, index, {role});
        return true;
    }

    return false;
}

} // namespace Slither

#endif // ENERGYPEARL_H
