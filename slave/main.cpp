#include <mpi.h>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <cmath>
#include <unistd.h>
#include <boost/mpi.hpp>
#include <boost/range/irange.hpp>
#include "Worker.hpp"
#include "IbcastExtensionForBoostMPI.hpp"

namespace mpi = boost::mpi;

static int numOfJobs;
static long problemSize;

inline const long segmentBegin(int rankForRange)
{
    return rankForRange * problemSize / (numOfJobs - 1);
}
inline const long segmentEnd(int rankForRange)
{
    return segmentBegin(rankForRange + 1);
}
inline const long segmentSize(int rankForRange)
{
    return segmentEnd(rankForRange) - segmentBegin(rankForRange);
}
inline const unsigned long getNumberFromIndex(unsigned long index, int rankForRange = 0)
{
    return (index + segmentBegin(rankForRange) + 1) * 2 + 1;
}
inline const unsigned long getIndexFromNumber(unsigned long number, int rankForRange = 0)
{
    return (number - segmentBegin(rankForRange) - 1) / 2 - 1;
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cerr << "Binary takes one and only one additional argument: size of range to seek for primes" << std::endl;
        return -1;
    }
    mpi::environment env(argc, argv);
    mpi::communicator world;
    numOfJobs = world.size();
    const long problemSize = std::ceil(std::stol(argv[1]) / 2.0) - 1;
    ::problemSize = problemSize;
    const int globalRank = world.rank();
    boost::mpi::communicator workersComm = world.split(globalRank && true);

    std::string buffer = "workersComm.size " + std::to_string(workersComm.size()) + '\n';
    std::cout << buffer << std::endl;
    int foremostWorker = 0;
    if (globalRank == 0)
    {
        foremostWorker = 1;
        mpi::request req[1] = {mpi::ibroadcast(world, foremostWorker, 0)};
//         mpi::wait_all(req, req + 1);
    }
    else if (globalRank > 0)
    {
        auto worker = Worker::getInstance(problemSize, workersComm);
        mpi::request req[1] = {mpi::ibroadcast(world, foremostWorker, 0)};
        auto broadcastCame = mpi::test_all(req, req + 1);
        buffer = "WorkerRank " + std::to_string(globalRank) + " ibroadcasted now, foremostWorker " + std::to_string(foremostWorker)
                 + ", broadcastCame " + std::to_string(broadcastCame) + "\n";
        std::cout << buffer;
        worker->run();
        const int workerRank = globalRank - 1;
//         while(true)
//         {
//             std::string buffer = "WorkerRank " + std::to_string(workerRank) + " starting broadcast now!\n";
//             std::cout << buffer;
//             broadcast(workersComm, foremostWorker, 1);
//
//             buffer = std::to_string(globalRank) + ": foremostWorker is " + std::to_string(foremostWorker) + "\n";
//             std::cout << buffer << std::endl;
//             std::vector<bool> localPart;
//             for(long i = 0; i < localPart.size(); ++i)
//             {
//                 localPart[i] = true;
//             }
//
//         }
        buffer = "Rank ";
        buffer += std::to_string(globalRank) + "/" + std::to_string(workerRank);
        buffer += " has beggining in ";
        buffer += std::to_string(segmentBegin(workerRank));
        buffer += ", end in ";
        buffer += std::to_string(segmentEnd(workerRank));
        buffer += ", size of ";
        buffer += std::to_string(segmentSize(workerRank));
        buffer += '\n';
        std::cout << buffer;
    }
    return 0;
}

