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
    void run();
private:
    typedef std::greater<const int> greater;
    typedef std::greater_equal<const int> greater_equal;
    typedef std::equal_to<const int> equal;
    typedef std::less<const int> less;
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

    template<typename T, typename Comparator>
    inline bool compareSafely(T lv, Comparator compare, T rv)
    {
        std::unique_lock<std::mutex> lock(workerLock);
        return compare(lv, rv);
    }

    void updatePrimes();
    void updateForemostWorker();

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
