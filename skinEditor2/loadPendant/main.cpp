#include "loader.h"

#include <QGuiApplication>
#include <QImage>
#include <QQmlApplicationEngine>


class Application : public QGuiApplication
{
public:
    using QGuiApplication::QGuiApplication;

    int run(QUrl qmlRoot = {});
};

int Application::run(QUrl qmlRoot)
{
    if(qmlRoot.isEmpty())
        return run({"qrc:/main.qml"});

    qmlRegisterType<PendantLoad::Loader>("PendantLoader", 1, 0, "Loader");
    QQmlApplicationEngine qml;
    qml.load(qmlRoot);
    if(qml.rootObjects().isEmpty())
        return EXIT_FAILURE;

    return exec();
}

int main(int argc, char *argv[])
{
    return Application{argc, argv}.run();
}
