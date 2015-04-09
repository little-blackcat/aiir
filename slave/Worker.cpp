#include "Worker.hpp"
#include <boost/mpi/collectives/reduce.hpp>
#include <algorithm>
#include <cmath>
#include <functional>

Worker* Worker::getInstance(const long lastNumber, const boost::mpi::communicator& comm)
{
    static Worker w(lastNumber, comm);
    return &w;
}

void Worker::run()
{
    precomputePrimes();
    removeMultiples();
    auto sumOfPrimes = sumPrimes();

    std::cout << "Worker " << rank << " finished with sum: " << sumOfPrimes << std::endl;
    long output = 0;
    boost::mpi::communicator world;
    boost::mpi::reduce(world, sumOfPrimes, output, std::plus<>(), 0);
}

Worker::Worker(const long lastNumber, const boost::mpi::communicator& comm) :
                    problemSize(lastNumber),
                    numOfWorkers(comm.size()),
                    workerCommunicator(comm),
                    rank(comm.rank()),
                    segmentBegin(calculateSegmentBegin(comm.rank(), lastNumber, comm.size())),
                    segmentEnd(calculateSegmentEnd(comm.rank(), lastNumber, comm.size())),
                    segmentSize(calculateSegmentSize(comm.rank(), lastNumber, comm.size()))
{
    localSegmentOfRange.resize(segmentSize, true);
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

void Worker::precomputePrimes()
{
    std::vector<long> smallSet;
    smallSet.resize(std::ceil(std::sqrt(problemSize * 2 + 1)), true);
    smallSet[0] = false;
    smallSet[1] = false;
    smallSet[2] = false;
    const auto size = smallSet.size();

    long prime = 2;
    while(prime < size)
    {
        primes.push(prime);
        for(int multiple = prime; multiple < size; multiple += prime)
        {
            smallSet[multiple] = false;
        }
        prime = std::distance(smallSet.begin(), std::find(smallSet.begin(), smallSet.end(), true));
    }
    primes.pop();
}

void Worker::removeMultiples()
{
    while(! primes.empty())
    {
        auto prime = primes.front();
        primes.pop();
        for(long i = getIndexFromNumber(firstUnevenMultipleInRange(prime)) + prime; i < segmentSize; i+= prime)
            {
                localSegmentOfRange[i] = false;
            }
    }
}

long Worker::sumPrimes()
{
    return std::count(localSegmentOfRange.begin(), localSegmentOfRange.end(), true);
}

