#ifndef PENDANTLOAD_LOADER_H
#define PENDANTLOAD_LOADER_H

#include <QObject>
#include <QUrl>

namespace PendantLoad {

class Loader : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString preview READ preview NOTIFY previewChanged FINAL)

    QStringList m_possiblePendants;

public:
    explicit Loader(QObject *parent = nullptr);

    QString preview() const;

public slots:
    void getPendant(QString path);

    int count();

    QString getPossiblePendant(int at);

    QUrl getUrl(QString pathInput);

signals:
    void previewChanged();
};

} // namespace PendantLoad

#endif // PENDANTLOAD_LOADER_H
