#include "Simulation.h"

#include <random>
#include <algorithm>

void Simulation::run(
        int numCustomers,
        std::vector<Customer>& customers,
        Statistics& stats)
{
    customers.clear();
    stats = Statistics();

    // Guards the divide-by-zero / empty-vector crash from indexing
    // customers.back() when numCustomers is 0 or negative.
    if (numCustomers < 1)
        return;

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<double> interDist(1.0, 8.0);
    std::uniform_real_distribution<double> serviceDist(1.0, 6.0);

    double totalWait = 0.0;
    double totalService = 0.0;
    double totalInterArrival = 0.0;
    double maxWait = 0.0;

    int waited = 0;

    customers.reserve(static_cast<std::size_t>(numCustomers));

    for (int i = 0; i < numCustomers; ++i)
    {
        Customer c;

        c.id = i + 1;

        c.interArrival = interDist(gen);
        c.serviceTime  = serviceDist(gen);

        if (i == 0)
        {
            c.arrival = c.interArrival;
            c.serviceStart = c.arrival;
            c.waitingTime = 0.0;
        }
        else
        {
            const Customer& prev = customers.back();

            c.arrival = prev.arrival + c.interArrival;
            c.serviceStart = std::max(c.arrival, prev.departure);
            c.waitingTime = c.serviceStart - c.arrival;
        }

        c.departure = c.serviceStart + c.serviceTime;

        totalWait += c.waitingTime;
        totalService += c.serviceTime;
        totalInterArrival += c.interArrival;

        if (c.waitingTime > 0.0)
            ++waited;

        maxWait = std::max(maxWait, c.waitingTime);

        customers.push_back(c);
    }

    const double n = static_cast<double>(numCustomers);
    const double simulationTime = customers.back().departure;

    stats.avgInterArrival = totalInterArrival / n;
    stats.avgServiceTime  = totalService / n;

    stats.avgWaitingTime  = totalWait / n;
    stats.avgSystemTime   = (totalWait + totalService) / n;

    stats.maxWaitingTime  = maxWait;
    stats.customersWhoWaited = waited;

    stats.utilization = (totalService / simulationTime) * 100.0;
}