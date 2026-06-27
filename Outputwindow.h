#ifndef OUTPUTWINDOW_H
#define OUTPUTWINDOW_H

#include <QWidget>

class QTableWidget;
class QTextEdit;

class OutputWindow : public QWidget
{
    Q_OBJECT

public:
    explicit OutputWindow(QWidget* parent = nullptr);

    // Re-runs the simulation and refreshes the table/stats in place,
    // so the same window can be reused across multiple runs.
    void runSimulation(int numCustomers);

private:
    QTableWidget* table;
    QTextEdit*    statsBox;
};

#endif