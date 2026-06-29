#include "OutputWindow.h"

#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QTextEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

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
    statsBox->setMaximumHeight(180);
    layout->addWidget(statsBox);

    exportButton = new QPushButton("Export Results to CSV");
    layout->addWidget(exportButton);

    connect(exportButton, &QPushButton::clicked,
            this, &OutputWindow::exportToCsv);
}

void OutputWindow::runSimulation(int numCustomers)
{
    Simulation::run(numCustomers, lastCustomers, lastStats);
    table->setRowCount(static_cast<int>(lastCustomers.size()));

    // One helper calls per row.
    auto cell = [](double value) {
        auto* item = new QTableWidgetItem(QString::number(value, 'f', 2));
        item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        return item;
    };

    for (std::size_t i = 0; i < lastCustomers.size(); ++i)
    {
        const Customer& c = lastCustomers[i];
        const int row = static_cast<int>(i);

        table->setItem(row, 0, new QTableWidgetItem(QString::number(c.id)));
        table->setItem(row, 1, cell(c.interArrival));
        table->setItem(row, 2, cell(c.arrival));
        table->setItem(row, 3, cell(c.serviceTime));
        table->setItem(row, 4, cell(c.waitingTime));
        table->setItem(row, 5, cell(c.departure));
    }
    statsBox->clear();
    statsBox->append(QString("Average Inter-arrival Time: %1 min").arg(lastStats.avgInterArrival, 0, 'f', 2));
    statsBox->append(QString("Average Service Time: %1 min").arg(lastStats.avgServiceTime, 0, 'f', 2));
    statsBox->append(QString("Average Waiting Time: %1 min").arg(lastStats.avgWaitingTime, 0, 'f', 2));
    statsBox->append(QString("Average Time in System: %1 min").arg(lastStats.avgSystemTime, 0, 'f', 2));
    statsBox->append(QString("Maximum Waiting Time: %1 min").arg(lastStats.maxWaitingTime, 0, 'f', 2));
    statsBox->append(QString("Customers Who Waited: %1 of %2").arg(lastStats.customersWhoWaited).arg(numCustomers));
    statsBox->append(QString("Server Utilization: %1 %").arg(lastStats.utilization, 0, 'f', 2));
    statsBox->append(QString("Server Idle Time: %1 min").arg(lastStats.totalIdleTime, 0, 'f', 2));
    statsBox->append(QString("Average Queue Length (Lq): %1 customers").arg(lastStats.queueLength, 0, 'f', 2));
}

void OutputWindow::exportToCsv()
{
    if (lastCustomers.empty())
    {
        QMessageBox::information(this, "Nothing to Export", "Run a simulation first.");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(
        this, "Export Results", "simulation_results.csv", "CSV Files (*.csv)");

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Export Failed", "Could not open the file for writing.");
        return;
    }
    QTextStream out(&file);
    out << "Customer,Inter-Arrival,Arrival,Service,Waiting,Departure\n";
    for (const Customer& c : lastCustomers)
    {
        out << c.id << ","
            << QString::number(c.interArrival, 'f', 2) << ","
            << QString::number(c.arrival, 'f', 2) << ","
            << QString::number(c.serviceTime, 'f', 2) << ","
            << QString::number(c.waitingTime, 'f', 2) << ","
            << QString::number(c.departure, 'f', 2) << "\n";
    }
    out << "\n";
    out << "Statistic,Value\n";
    out << "Average Inter-arrival Time," << QString::number(lastStats.avgInterArrival, 'f', 2) << "\n";
    out << "Average Service Time," << QString::number(lastStats.avgServiceTime, 'f', 2) << "\n";
    out << "Average Waiting Time," << QString::number(lastStats.avgWaitingTime, 'f', 2) << "\n";
    out << "Average Time in System," << QString::number(lastStats.avgSystemTime, 'f', 2) << "\n";
    out << "Maximum Waiting Time," << QString::number(lastStats.maxWaitingTime, 'f', 2) << "\n";
    out << "Customers Who Waited," << lastStats.customersWhoWaited << "\n";
    out << "Server Utilization (%)," << QString::number(lastStats.utilization, 'f', 2) << "\n";
    out << "Server Idle Time," << QString::number(lastStats.totalIdleTime, 'f', 2) << "\n";
    out << "Average Queue Length (Lq)," << QString::number(lastStats.queueLength, 'f', 2) << "\n";

    file.close();
    QMessageBox::information(this, "Export Successful", "Results exported to:\n" + fileName);
}