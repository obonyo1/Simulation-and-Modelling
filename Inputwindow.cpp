#include "InputWindow.h"
#include "OutputWindow.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QIntValidator>

InputWindow::InputWindow()
{
    setWindowTitle("Queue Simulation Input");

    auto* layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("Number of Customers (1-100000):"));

    customerInput = new QLineEdit("100");
    customerInput->setValidator(new QIntValidator(1, 100000, customerInput));
    layout->addWidget(customerInput);

    simulateButton = new QPushButton("Run Simulation");
    layout->addWidget(simulateButton);

    connect(simulateButton, &QPushButton::clicked,
            this, &InputWindow::runSimulation);
}

void InputWindow::runSimulation()
{
    int n = customerInput->text().toInt();

    // The validator already restricts the field to 1-100000, but this
    // guards the rare case of an empty/cleared field reaching here as 0.
    if (n < 1)
        return;

    if (!outputWindow)
    {
        outputWindow = new OutputWindow();
        outputWindow->setAttribute(Qt::WA_DeleteOnClose);

        // Drop the pointer if the user closes the results window, so the
        // next run creates a fresh one instead of touching a deleted object.
        connect(outputWindow, &QObject::destroyed, this, [this]() {
            outputWindow = nullptr;
        });
    }

    outputWindow->runSimulation(n);
    outputWindow->show();
    outputWindow->raise();
    outputWindow->activateWindow();
}