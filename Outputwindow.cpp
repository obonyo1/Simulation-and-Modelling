#include "OutputWindow.h"
#include "Simulation.h"

#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QTextEdit>

OutputWindow::OutputWindow(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle("Simulation Results");
    resize(1100, 700);

    auto* layout = new QVBoxLayout(this);

    table = new QTableWidget();
    table->setColumnCount(6);
    table->setHorizontalHeaderLabels({
        "Customer", "Inter-Arrival", "Arrival",
        "Service", "Waiting", "Departure"
    });
    table->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(table);

    statsBox = new QTextEdit();
    statsBox->setReadOnly(true);
    statsBox->setMaximumHeight(160);
    layout->addWidget(statsBox);
}

void OutputWindow::runSimulation(int numCustomers)
{
    std::vector<Customer> customers;
    Statistics stats;

    Simulation::run(numCustomers, customers, stats);

    table->setRowCount(static_cast<int>(customers.size()));

    // One helper instead of six near-identical setItem calls per row.
    auto cell = [](double value) {
        auto* item = new QTableWidgetItem(QString::number(value, 'f', 2));
        item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        return item;
    };

    for (std::size_t i = 0; i < customers.size(); ++i)
    {
        const Customer& c = customers[i];
        const int row = static_cast<int>(i);

        table->setItem(row, 0, new QTableWidgetItem(QString::number(c.id)));
        table->setItem(row, 1, cell(c.interArrival));
        table->setItem(row, 2, cell(c.arrival));
        table->setItem(row, 3, cell(c.serviceTime));
        table->setItem(row, 4, cell(c.waitingTime));
        table->setItem(row, 5, cell(c.departure));
    }

    statsBox->clear();
    statsBox->append(QString("Average Inter-arrival Time: %1 min").arg(stats.avgInterArrival, 0, 'f', 2));
    statsBox->append(QString("Average Service Time: %1 min").arg(stats.avgServiceTime, 0, 'f', 2));
    statsBox->append(QString("Average Waiting Time: %1 min").arg(stats.avgWaitingTime, 0, 'f', 2));
    statsBox->append(QString("Average Time in System: %1 min").arg(stats.avgSystemTime, 0, 'f', 2));
    statsBox->append(QString("Maximum Waiting Time: %1 min").arg(stats.maxWaitingTime, 0, 'f', 2));
    statsBox->append(QString("Customers Who Waited: %1 of %2").arg(stats.customersWhoWaited).arg(numCustomers));
    statsBox->append(QString("Server Utilization: %1 %").arg(stats.utilization, 0, 'f', 2));
}