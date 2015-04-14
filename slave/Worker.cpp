#include "Worker.hpp"
#include <boost/mpi/collectives/reduce.hpp>
#include <algorithm>
#include <cmath>
#include <functional>


Worker& Worker::getInstance(const long lastNumber, const boost::mpi::communicator& comm)
{
    static Worker w(lastNumber, comm);
    return w;
}

void Worker::run()
{
    precomputePrimes();
    removeMultiples();
    auto sumOfPrimes = sumPrimes();
    std::cout << "Worker " << rank << " finished." << std::endl;
    boost::mpi::communicator world;
    boost::mpi::reduce(world, sumOfPrimes, std::plus<>(), 0);
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
    if(! (firstMultiple % 2))
    {
        firstMultiple += prime;
    }
    else if(firstMultiple == prime)
    {
        firstMultiple *= prime;
    }
    return firstMultiple;
}

void Worker::precomputePrimes()
{
    std::vector<bool> smallSet;
    smallSet.resize(std::ceil(std::sqrt(problemSize * 2 + 1)), true);
    smallSet[0] = false;
    smallSet[1] = false;
    smallSet[2] = false;
    const auto size = smallSet.size();

    long prime = 2;
    while(prime < size)
    {
        primes.push(prime);
        smallSet[prime] = false;
        for(auto multiple = prime * prime; multiple < size; multiple += prime)
        {
            smallSet[multiple] = false;
        }
        prime = std::distance(smallSet.begin(), std::find(smallSet.begin() + prime, smallSet.end(), true));
    }
    primes.pop(); // pop 2 from queue
}

void Worker::removeMultiples()
{
    while(! primes.empty())
    {
        const auto prime = primes.front();
        primes.pop();
        for(auto i = getIndexFromNumber(firstUnevenMultipleInRange(prime)); i < segmentSize; i+= prime)
            {
                localSegmentOfRange[i] = false;
            }
    }
}

long Worker::sumPrimes()
{
    return std::count(localSegmentOfRange.begin(), localSegmentOfRange.end(), true);
}

