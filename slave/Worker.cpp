#include "Worker.hpp"

namespace mpi = boost::mpi;

Worker* Worker::getInstance(const long lastNumber, const mpi::communicator& comm)
{
    static Worker w(lastNumber, comm);
    return &w;
}

Worker::Worker(const long int lastNumber, const mpi::communicator& comm) :
                    problemSize(lastNumber),
                    numOfWorkers(comm.size()),
                    workerCommunicator(comm),
                    rank(comm.rank())
{
    localSegmentOfRange.resize(segmentSize(rank), false);
    primes.push(3);
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
