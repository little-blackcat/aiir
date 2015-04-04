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

    if (globalRank == 0)
    {
        int foremostWorker = 1;
        broadcast(world, foremostWorker, globalRank);
    }
    else if (globalRank > 0)
    {
        auto w = Worker::getInstance(problemSize, workersComm);
        int foremostWorker = 0;
        broadcast(world, foremostWorker, foremostWorker);
        const int workerRank = globalRank - 1;
//         mpi::communicator workersComm(world, workers);
        while(true)
        {
            std::string buffer = "WorkerRank " + std::to_string(workerRank) + " starting broadcast now!\n";
            std::cout << buffer;
            broadcast(workersComm, foremostWorker, 1);
//             std::string buffer = std::to_string(globalRank) + ": MPI_Bcast(&newForemostWorker, 1, MPI_INT, " + std::to_string(foremostWorker) + ", MPI_COMM_WORLD);\n";
//             std::cout << buffer;

            buffer = std::to_string(globalRank) + ": foremostWorker is " + std::to_string(foremostWorker) + "\n";
            std::cout << buffer << std::endl;
            std::vector<bool> localPart;
            localPart.resize(segmentSize(workerRank), false);
//             std::queue<long> indexesOfFoundPrimes;
//             indexesOfFoundPrimes.insert(0);
            for(long i = 0; i < localPart.size(); ++i)
            {
                localPart[i] = true;
            }
//             if(foremostWorker++ == globalRank)
//             {
//                 std::string buffer;
//                 buffer = std::to_string(globalRank) + ": MPI_Bcast(&nextForemostWorker, 1, MPI_INT, " + std::to_string(globalRank) + ", MPI_COMM_WORLD);\n";
//                 std::cout << buffer << std::endl;
//                 broadcast(world, foremostWorker, globalRank);
//             }

        }
        std::string buffer = "Rank ";
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

