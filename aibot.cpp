#include "aibot.h"

#include "playground.h"
#include "snake.h"

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
qreal __globalNet_fitness = 1;

NeuralNet *globalNet()
{
    if(Q_UNLIKELY(!__globalNet)) {
        __globalNet = new NeuralNet(9, 3, 3, 10);
        QString name = _32bitHexIntStr(std::uintptr_t(__globalNet));
    }

    return __globalNet;
}

void applyMutation(QPair<NeuralNet*, qreal> best)
{
    if(best.second < (__globalNet_fitness * .75)) // must reach at least 75% of the current highscore to get counted
        return;

    if(best.second > __globalNet_fitness) {// this one was better
        __globalNet_fitness = best.second;
        __globalNet->assign(best.first);
        return;
    }

    qreal weightBest = best.second / __globalNet_fitness * .75;

    __globalNet->crossover(best.first, weightBest);
}

}

AiBot::AiBot(Snake *parent)
    : Bot(parent)
    , m_net(globalNet())
{
    m_net->mutate(0.2);

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

    /*
    auto netAddr = (std::uintptr_t)m_net;
    QString name = _32bitHexIntStr(netAddr);

    auto f = new QFile(name + ".neuralNet");
    f->open(QFile::ReadWrite);
    QDataStream stream(f);
    stream << 1;
    */

    connect(parent, &Snake::died, this, &AiBot::die);
//    stream << m_net;
}

namespace
{

bool comp(QPair<qreal, Snake*> a, QPair<qreal, Snake*> b)
{
    return a.first < b.first;
}

QPair<NeuralNet *, qreal> getBestSnake(QList<Snake *> l)
{
    qInfo() << "getting best snake";
    QList<QPair<qreal, Snake*>> d = {};

    for(Snake *s: l)
        if(s->botType() == Bot::Bot_Intelligent) {
            qreal value = 0;
            value += reinterpret_cast<AiBot *>(s->bot())->age();
            value += s->length();

            value -= reinterpret_cast<AiBot *>(s->bot())->starvation();

            d += QPair<qreal, Snake*>(value, s);
        }

    std::sort(d.begin(), d.end(), comp);
    return {const_cast<NeuralNet*>(reinterpret_cast<AiBot *> (d[0].second->bot())->net()), d[0].first}; // ... yes long and messy code
}

}

void AiBot::die()
{
    qInfo() << Q_FUNC_INFO;

    QList<Snake*> bestList = playground()->leaderboard()->leaderboard();
    if(bestList.isEmpty())
        return;

    auto bestNet = getBestSnake(bestList);
    applyMutation(bestNet);
}

NeuralNet *AiBot::global()
{
    return globalNet();
}

bool AiBot::maybeStarve()
{
    static qreal prevLen = snake()->length();

    const int maxTime = 500; // tweak this

    bool ret = false;

    if(prevLen >= snake()->length()) {
        ++m_starvingTime;

        if(m_starvingTime >= maxTime) {
            playground()->killSnake(snake());
            ret = true;
        }
    }
    else { // the snake grew
        m_starvingTime = 0;
    }

    prevLen = snake()->length();

    return ret;
}

void AiBot::act(qreal dt)
{
    if(maybeStarve()) {
        return;
    }
    m_age += dt / 100.;

    QList<qreal> input;

    const qreal a = atan2(m_snake->direction().y(), m_snake->direction().x()); // the angle the snake is currently facing, calculate it here so we can use it faster

    // data about yourself
    input.append(m_snake->boosting());
    input.append(atan2(m_snake->direction().y(), m_snake->direction().x()) / M_PI - a); // we use radiants, since I don't want to convert them to degrees

    // data about the border
    input.append((playground()->size() - QVector2D(m_snake->position()).length()) / playground()->size()); // distance

    const auto borderRel = findBorder() - position();
    input.append(atan2(borderRel.y(), borderRel.x()) / M_PI - a); // angle to border


    // data about the next food

    const auto nextFood = findNextFood();
    const auto foodRelPos = nextFood.position - position();

    input.append(QVector2D(foodRelPos).length() / playground()->size()); // distance
    input.append(atan2(foodRelPos.y(), foodRelPos.x()) / M_PI - a); // angle

    input.append(nextFood.amount); // value


    // data about the closest snake segment

    const auto snakeSegRel = findNextSnakeSegment() - position();

    input.append(QVector2D(snakeSegRel).length() / playground()->size()); // distance
    input.append(atan2(snakeSegRel.y(), snakeSegRel.x()) / M_PI - a); // angle to segment


    // use the output
    auto output = m_net->decide(input);

    m_snake->setBoosting(output[0] > 2);

    m_snake->setDestination(m_snake->position() + QPointF{cos((output[2] - output[1]) * M_PI + a), sin((output[2] - output[1]) * M_PI + a)});
}

} // namespace Slither
