#ifndef INPUTWINDOW_H
#define INPUTWINDOW_H

#include <QWidget>

class QLineEdit;
class QPushButton;
class OutputWindow;

class InputWindow : public QWidget
{
    Q_OBJECT

public:
    InputWindow();

private:
    QLineEdit*    customerInput;
    QPushButton*  simulateButton;

    // Reused across runs so clicking "Run" again updates the same
    // output window instead of leaking a new one each time.
    OutputWindow* outputWindow = nullptr;

private slots:
    void runSimulation();
};

#endif