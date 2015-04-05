#include <memory>
#include <boost/mpi.hpp>
#include <queue>
#include <vector>
#include <mutex>
#include <cmath>
#include "IbcastExtensionForBoostMPI.hpp"
#pragma once

namespace mpi = boost::mpi;
class Worker
{
public:
    static Worker* getInstance(const long int lastNumber, const mpi::communicator& comm);
private:
    enum operationTag : int
    {
        advanceForemostWorker = 42
    };
    Worker(const long lastNumber, const mpi::communicator& comm);
    inline const long segmentBegin(const int rankForRange);
    inline const long segmentEnd(const int rankForRange);
    inline const long segmentSize(const int rankForRange);
    inline const unsigned long getNumberFromIndex(const long index, int rankForRange);
    inline const unsigned long getIndexFromNumber(const long number, int rankForRange);
    void run();
    Worker(Worker const&) = delete;
    void operator=(Worker const&) = delete;
    const long problemSize;
    const int numOfWorkers;
    int foremostWorker = 0;
    const mpi::communicator workerCommunicator;
    const int rank;
    std::vector<bool> localSegmentOfRange;
    std::vector<bool>::iterator foundPrime = localSegmentOfRange.begin();
    std::queue<long> primes;
    std::mutex queueLock;
    std::mutex workerLock;
};
