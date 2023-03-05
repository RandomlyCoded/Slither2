#ifndef AIBOT_NETBASE_H

#define AIBOT_NETBASE_H

#define PERCENT / 100.0

#include <NeuralNet>
#include <QObject>

namespace Slither
{

class NetBase : public QObject
{
    Q_OBJECT

public:
    NetBase();
    virtual void act(qreal dt) = 0;

    enum class Type {
        NetA = 100,
        NetB = 200,
        NoNet = -1,
        Amt = 2,
    };
    Q_ENUM(Type);

    virtual const Type type() const = 0;
    constexpr static qreal MUTATION_RATE() { return 20 PERCENT; }

    const NeuralNet *net() const { return m_net; }

protected:
    NeuralNet *m_net = nullptr;
};

} // namespace Slither

#endif // AIBOT_NETBASE_H
