#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>

struct Customer
{
    int id;

    double interArrival;
    double arrival;

    double serviceTime;

    double serviceStart;
    double waitingTime;

    double departure;
};

struct Statistics
{
    double avgInterArrival   = 0.0;
    double avgServiceTime    = 0.0;

    double avgWaitingTime    = 0.0;
    double avgSystemTime     = 0.0;

    double maxWaitingTime    = 0.0;
    int    customersWhoWaited = 0;

    double utilization       = 0.0;

    double totalIdleTime     = 0.0;  // simulationTime - totalService
    double queueLength       = 0.0;  // Lq: time-weighted average number waiting in line
};

class Simulation
{
public:
    static void run(
        int numCustomers,
        std::vector<Customer>& customers,
        Statistics& stats);
};

#endif