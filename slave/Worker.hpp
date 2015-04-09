#include <boost/mpi/communicator.hpp>
#include <queue>
#include <vector>

#pragma once

class Worker
{
public:
    static Worker* getInstance(const long int lastNumber, const boost::mpi::communicator& comm);
    void run();
private:

    Worker(const long lastNumber, const boost::mpi::communicator& comm);

    inline const long calculateSegmentBegin(const int rank, const long int problemSize, const long int numOfWorkers);
    inline const long calculateSegmentEnd(const int rank, const long int problemSize, const long int numOfWorkers);
    inline const long calculateSegmentSize(const int rank, const long int problemSize, const long int numOfWorkers);
    inline const long getNumberFromIndex(const long index);
    inline const long getIndexFromNumber(const long number);
    inline const long firstUnevenMultipleInRange(const long prime);

    void precomputePrimes();
    void removeMultiples();
    long sumPrimes();

    Worker(Worker const&) = delete;
    void operator=(Worker const&) = delete;

    const long problemSize;
    const int numOfWorkers;
    const long segmentBegin;
    const long segmentEnd;
    const long segmentSize;
    const boost::mpi::communicator workerCommunicator;
    const int rank;
    std::vector<bool> localSegmentOfRange;
    std::queue<long> primes;
    long primesFound;
};
