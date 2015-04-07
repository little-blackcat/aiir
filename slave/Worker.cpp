#include "Worker.hpp"
#include <functional>
#include <thread>

namespace mpi = boost::mpi;

namespace threads
{
    using namespace std::literals;
    auto idleTime = 50us;
}

Worker* Worker::getInstance(const long lastNumber, const mpi::communicator& comm)
{
    static Worker w(lastNumber, comm);
    return &w;
}

void Worker::run()
{
    std::thread primeUpdater(&Worker::updatePrimes, this);
    std::thread workerUpdater(&Worker::updateForemostWorker, this);

    while(rank > foremostWorker)
    {
        removeMultiples();
    }

    primeUpdater.join();
    workerUpdater.join();

    while(rank == foremostWorker)
    {
        bool allMultiplesRemoved = removeMultiples();
        if(lastFoundPrime != localSegmentOfRange.end() && allMultiplesRemoved)
        {
            findNextPrime();
        }
        else if(lastFoundPrime == localSegmentOfRange.end() && allMultiplesRemoved)
        {
            advanceForemostWorker();
        }
    }
    std::cout << "Worker " << rank << " finished\n";
}

Worker::Worker(const long lastNumber, const mpi::communicator& comm) :
                    problemSize(lastNumber),
                    numOfWorkers(comm.size()),
                    workerCommunicator(comm),
                    rank(comm.rank()),
                    segmentBegin(calculateSegmentBegin(comm.rank(), lastNumber, comm.size())),
                    segmentEnd(calculateSegmentEnd(comm.rank(), lastNumber, comm.size())),
                    segmentSize(calculateSegmentSize(comm.rank(), lastNumber, comm.size()))
{
    foremostWorker = 0;
    localSegmentOfRange.resize(segmentSize, true);
    lastFoundPrime = localSegmentOfRange.begin();
    primeBroadcastRequests.resize(numOfWorkers);
    std::cout << "Worker " << rank << " started up!\n";
}

inline const long Worker::calculateSegmentBegin(const int rank, const long problemSize, const long numOfWorkers)
{
    return rank * problemSize / numOfWorkers;
}
inline const long Worker::calculateSegmentEnd(const int rank, const long problemSize, const long numOfWorkers)
{
    return (rank + 1) * problemSize / numOfWorkers;
}
inline const long Worker::calculateSegmentSize(const int rank, const long problemSize, const long numOfWorkers)
{
    return ((rank + 1) * problemSize / numOfWorkers) - (rank * problemSize / numOfWorkers);
}
inline const long Worker::getNumberFromIndex(const long index)
{
    return (index + segmentBegin + 1) * 2 + 1;
}
inline const long Worker::getIndexFromNumber(const long number)
{
    return (number - getNumberFromIndex(0)) / 2 ;
}
inline const long Worker::firstUnevenMultipleInRange(const long prime)
{
    long remainder = getNumberFromIndex(0) % prime;
    long firstMultiple = getNumberFromIndex(0) + (prime - remainder) % prime;
    return firstMultiple;
}

void Worker::updatePrimes()
{
    while(rank > foremostWorker)
    {
        std::this_thread::sleep_for(threads::idleTime);
        int previousForemostWorker = foremostWorker;
        if(workerCommunicator.iprobe(previousForemostWorker, primeBroadcast))
        {
            long prime;
            workerCommunicator.recv(previousForemostWorker, primeBroadcast, prime);
            std::unique_lock<std::mutex> lock(queueLock);
            primes.push(prime);
        }
    }
}

void Worker::updateForemostWorker()
{
    while(rank > foremostWorker)
    {
        std::this_thread::sleep_for(threads::idleTime);
        if(workerCommunicator.iprobe(foremostWorker, foremostWorkerChange))
        {
            int newForemostWorker;
            workerCommunicator.recv(foremostWorker, foremostWorkerChange, newForemostWorker);
            foremostWorker = newForemostWorker;
        }
    }
}

void Worker::findNextPrime()
{
    if((lastFoundPrime = std::find(lastFoundPrime, localSegmentOfRange.end(), true)) != localSegmentOfRange.end())
    {
        auto index = std::distance(localSegmentOfRange.begin(), lastFoundPrime);
        auto prime = getNumberFromIndex(index);
        std::unique_lock<std::mutex> lock(queueLock);
        primes.push(prime);
        lock.unlock();
        mpi::wait_all(primeBroadcastRequests.begin() + rank + 1, primeBroadcastRequests.end());
        for (int i = rank + 1; i < numOfWorkers; ++i)
        {
            primeBroadcastRequests[i] = workerCommunicator.isend(i, primeBroadcast, prime);
        }
    }
}

bool Worker::removeMultiples()
{
    using namespace std::literals;
    std::unique_lock<std::mutex> lock(queueLock);
    if(! primes.empty())
    {
        const long prime = primes.front();
        primes.pop();
        lock.unlock();
        for(long i = getIndexFromNumber(firstUnevenMultipleInRange(prime)); i < segmentSize; i+= prime)
        {
            localSegmentOfRange[i] = false;
        }
        return false;
    }
    return true;
}

void Worker::advanceForemostWorker()
{
    foremostWorker++;
    std::vector<mpi::request> req;
    for(int i = rank + 1; i < numOfWorkers; ++i)
    {
        req.push_back(workerCommunicator.isend(i, foremostWorkerChange, foremostWorker.load()));
    }
    mpi::wait_all(req.begin(), req.end());
    std::cout << "foremostWorker advanced to " << foremostWorker.load() << "\n";
}
