#include "aibot.h"

#include "playground.h"
#include "snake.h"

#include <neuralnetviewprovider.h>

#include <QFile>
#include <QVector2D>
#include <neuralnet.h>

namespace Slither {

namespace
{

#define fastAp(i) case 0x##i: str += #i; break;

QString _32bitHexIntStr(uint64_t _i)
{
    QString str = "0x";
    for(int i = 29; i > 0; i -= 4) {
        switch(unsigned(_i & (0b1111 << i)) >> i) { // need to shift it back
        fastAp(0)
        fastAp(1)
        fastAp(2)
        fastAp(3)
        fastAp(4)
        fastAp(5)
        fastAp(6)
        fastAp(7)
        fastAp(8)
        fastAp(9)
        fastAp(a)
        fastAp(b)
        fastAp(c)
        fastAp(d)
        fastAp(e)
        fastAp(f)
        }
    }

    return str;
}

NeuralNet *__globalNet = nullptr;

NeuralNet *globalNet()
{
    if(Q_UNLIKELY(!__globalNet)) {
        __globalNet = new NeuralNet(9, 3, 3, 10);
        QString name = _32bitHexIntStr(std::uintptr_t(__globalNet));
        NeuralNetViewProvider().requestNetView(__globalNet).save("globalNet-" + name + ".png", "png");
    }

    return __globalNet;
}

}

AiBot::AiBot(Snake *parent)
    : Bot(parent)
    , m_net(globalNet()->clone())
{
    m_net->mutate(0.5);

    /********************************************
     * boosting                     ->  -> boosting
     * facing angle                 ->  -> left
     * border distance              ->  -> right
     * border angle                 ->
     * next food distance           ->
     * next food angle              ->
     * next food value              ->
     * next snake segment distance  ->
     * next snake segment angle     ->
     *
     * left and right means how much it wants to go to wich side
     */
    auto netAddr = (std::uintptr_t)m_net;
    QString name = _32bitHexIntStr(netAddr);
    NeuralNetViewProvider().requestNetView(m_net).save(name + ".png", "png");

    auto f = new QFile(name + ".neuralNet");
    f->open(QFile::ReadWrite);
    QDataStream stream(f);
    stream << 1;
//    stream << m_net;
}

namespace
{

bool comp(QPair<qreal, Snake*> a, QPair<qreal, Snake*> b)
{
    return a.first < b.first;
}

NeuralNet *getBestSnake(QList<Snake *> l)
{
    QList<QPair<qreal, Snake*>> d = {};

    for(Snake *s: l)
        if(s->botType() == Bot::Bot_Intelligent) {
            qreal value = 0;
            value += reinterpret_cast<AiBot *>(s->bot())->age();
            value += s->length();
            d += QPair<qreal, Snake*>(value, s);
        }

    std::sort(d.begin(), d.end(), comp);
    return const_cast<NeuralNet*>(reinterpret_cast<AiBot *>(d[0].second->bot())->net()); // ... yes long and messy code
}

}

void AiBot::die()
{
    QList<Snake*> bestList = playground()->leaderboard()->leaderboard();
    if(bestList.isEmpty())
        return;

    NeuralNet *bestNet = getBestSnake(bestList);
    globalNet()->crossover(bestNet);
}

void AiBot::act(qreal dt)
{
    m_age += dt / 1e6;

    QList<qreal> input;

    // data about yourself
    input.append(m_snake->boosting());
    input.append(atan2(m_snake->direction().y(), m_snake->direction().x())); // we use radiants, since I don't want to convert them to degrees

    // data about the border
    input.append(playground()->size() - QVector2D(m_snake->position()).length()); // distance

    const auto borderRel = findBorder() - position();
    input.append(atan2(borderRel.y(), borderRel.x())); // angle to border

    // data about the next food
    input.append(playground()->size() - QVector2D(m_snake->position()).length()); // distance

    const auto nextFood = findNextFood();
    const auto foodRelPos = nextFood.position - position();
    input.append(atan2(foodRelPos.y(), foodRelPos.x())); // angle

    input.append(nextFood.amount); // value

    // data about the border
    input.append(playground()->size() - QVector2D(m_snake->position()).length()); // distance

    const auto snakeSegRel = findNextSnakeSegment() - position();
    input.append(atan2(snakeSegRel.y(), snakeSegRel.x())); // angle to segment

    auto output = m_net->decide(input);

    m_snake->setBoosting(output[0] > 0);

    m_snake->setDestination(m_snake->position() + QPointF{cos(output[2] - output[1]), sin(output[2] - output[1])});

    qInfo() << output;

//    qInfo() << m_snake->destination() << m_snake->boosting();
}

} // namespace Slither
