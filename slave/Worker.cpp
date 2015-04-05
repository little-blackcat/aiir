#include "Worker.hpp"
#include <functional>

namespace mpi = boost::mpi;

Worker* Worker::getInstance(const long lastNumber, const mpi::communicator& comm)
{
    static Worker w(lastNumber, comm);
    return &w;
}

void Worker::run()
{
    //threads for updating primes and foremostWorker
    while(compareSafely(rank, greater_equal(), foremostWorker))
    {
        if(compareSafely(rank, equal(), foremostWorker) && foundPrime != localSegmentOfRange.end())
        {
            auto index = std::distance(localSegmentOfRange.begin(), foundPrime);
            long prime = getNumberFromIndex(index, rank);
            std::unique_lock<std::mutex> lock(queueLock);
            primes.push(prime);
            lock.unlock();
            ibroadcast(workerCommunicator, prime, rank);
            foundPrime = std::find(foundPrime, localSegmentOfRange.end(), true);
        }

        std::unique_lock<std::mutex> lock(queueLock);
        if(! primes.empty())
        {
            const long prime = primes.front();
            primes.pop();
            lock.unlock();
            for(long i = getIndexFromNumber(prime, rank); i < segmentSize(rank); i *= prime)
            {
                localSegmentOfRange[i] = false;
            }
        }
    }

    foremostWorker++;
    for(int i = 0; i < numOfWorkers; ++i)
    {
        if(i != rank)
        {
            workerCommunicator.isend(i, operationTag::advanceForemostWorker, foremostWorker);
        }
    }

    while(compareSafely(rank, less(), foremostWorker) && compareSafely(foremostWorker, less(), numOfWorkers))
    {
        long prime;
        mpi::request req[2];
        req[0] = mpi::ibroadcast(workerCommunicator, prime, foremostWorker);
        req[1] = workerCommunicator.irecv(foremostWorker, operationTag::advanceForemostWorker, foremostWorker);
        mpi::wait_any(req, req + 1);
    }
}

Worker::Worker(const long int lastNumber, const mpi::communicator& comm) :
                    problemSize(lastNumber),
                    numOfWorkers(comm.size()),
                    workerCommunicator(comm),
                    rank(comm.rank())
{
    localSegmentOfRange.resize(segmentSize(rank), false);
}

inline const long Worker::segmentBegin(const int rankForRange)
{
    return rankForRange * problemSize / numOfWorkers;
}
inline const long Worker::segmentEnd(const int rankForRange)
{
    return segmentBegin(rankForRange + 1);
}
inline const long Worker::segmentSize(const int rankForRange)
{
    return segmentEnd(rankForRange) - segmentBegin(rankForRange);
}
inline const unsigned long Worker::getNumberFromIndex(const long index, int rankForRange)
{
    return (index + segmentBegin(rankForRange) + 1) * 2 + 1;
}
inline const unsigned long Worker::getIndexFromNumber(const long number, int rankForRange)
{
    return (number - segmentBegin(rankForRange) - 1) / 2 - 1;
}

void Worker::updatePrimes()
{
    
}

void Worker::updateForemostWorker()
{

}
