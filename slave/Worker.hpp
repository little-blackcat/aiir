#include <memory>
#include <boost/mpi.hpp>
#include <queue>
#include <vector>
#include <mutex>
#include <cmath>
#include <atomic>
#include "IbcastExtensionForBoostMPI.hpp"

#pragma once

namespace mpi = boost::mpi;
class Worker
{
public:
    static Worker* getInstance(const long int lastNumber, const mpi::communicator& comm);
    void run();
private:
//     typedef std::greater<const int> greater;
    enum operationTag : int
    {
        primeBroadcast = 24,
        foremostWorkerChange = 42
    };

    Worker(const long lastNumber, const mpi::communicator& comm);

    inline const long calculateSegmentBegin(const int rank, const long int problemSize, const long int numOfWorkers);
    inline const long calculateSegmentEnd(const int rank, const long int problemSize, const long int numOfWorkers);
    inline const long calculateSegmentSize(const int rank, const long int problemSize, const long int numOfWorkers);
    inline const long getNumberFromIndex(const long index);
    inline const long getIndexFromNumber(const long number);
    inline const long firstUnevenMultipleInRange(const long prime);

//     template<typename T, typename Comparator>
//     inline bool compareSafely(const T& lv, Comparator compare, const T& rv)
//     {
//         std::unique_lock<std::mutex> lock(workerLock);
//         return compare(lv, rv);
//     }
    void updatePrimes();
    void updateForemostWorker();

    void findNextPrime();
    bool removeMultiples();
    void advanceForemostWorker();

    Worker(Worker const&) = delete;
    void operator=(Worker const&) = delete;

    const long problemSize;
    const int numOfWorkers;
    const long segmentBegin;
    const long segmentEnd;
    const long segmentSize;
    std::atomic<int> foremostWorker;
    const mpi::communicator workerCommunicator;
    std::vector<mpi::request> primeBroadcastRequests;
    const int rank;
    std::vector<bool> localSegmentOfRange;
    std::vector<bool>::iterator lastFoundPrime;
    std::queue<long> primes;
    std::mutex queueLock;
};
