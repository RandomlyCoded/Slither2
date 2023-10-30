#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "editor.h"

class Application : public QGuiApplication
{
public:
    using QGuiApplication::QGuiApplication;

    int run(QUrl qmlroot = {});
};

int Application::run(QUrl qmlroot)
{
    QQmlApplicationEngine qml;

    qmlRegisterType<Editor::Data>("Editor", 1, 0, "Data");

    qml.load(qmlroot);
    if (qml.rootObjects().isEmpty())
        return EXIT_FAILURE;

    return exec();
}

int main(int argc, char *argv[])
{
    return Application{argc, argv}.run(QUrl {"qrc:/main.qml"});
}
