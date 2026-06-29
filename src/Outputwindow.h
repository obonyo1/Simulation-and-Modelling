#ifndef OUTPUTWINDOW_H
#define OUTPUTWINDOW_H

#include <QWidget>
#include "Simulation.h"

class QTableWidget;
class QTextEdit;
class QPushButton;

class OutputWindow : public QWidget
{
    Q_OBJECT

public:
    explicit OutputWindow(QWidget* parent = nullptr);

    // Re-runs the simulation and refreshes the table
    void runSimulation(int numCustomers);

private:
    QTableWidget* table;
    QTextEdit*    statsBox;
    QPushButton*  exportButton;

    //the export button can write out what was last run.
    std::vector<Customer> lastCustomers;
    Statistics             lastStats;

private slots:
    void exportToCsv();
};

#endif