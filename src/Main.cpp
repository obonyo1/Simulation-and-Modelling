#include <QApplication>
#include "InputWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    InputWindow window;

    window.resize(380, 180);
    window.show();

    return app.exec();
}