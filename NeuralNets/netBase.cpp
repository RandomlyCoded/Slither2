#include "netBase.h"

namespace Slither
{

NetBase::NetBase()
    : m_net(new NeuralNet(6, 10, 2, 2))
{}

} // namespace Slither
