#include <mpi.h>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <cmath>
#include <unistd.h>
#include <boost/mpi.hpp>

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
    mpi::group workers(world.group(), false);
    mpi::communicator workersComm(world, workers.exclude(0, 1));
    numOfJobs = world.size();
    problemSize = std::ceil(std::stol(argv[1]) / 2.0) - 1;

    const int rank = world.rank();
    if (rank == 0)
    {
        int foremostWorker = 1;
        broadcast(world, foremostWorker, rank);
    }
    else if (rank > 0)
    {
        int foremostWorker = 0;
        const int rankForRange = rank - 1;
        while(foremostWorker < rank)
        {
            std::string buffer = std::to_string(rank) + ": MPI_Bcast(&newForemostWorker, 1, MPI_INT, " + std::to_string(foremostWorker) + ", MPI_COMM_WORLD);\n";
            std::cout << buffer;
            broadcast(world, foremostWorker, foremostWorker);
            buffer = std::to_string(rank) + ": foremostWorker is " + std::to_string(foremostWorker) + "\n";
            std::cout << buffer << std::endl;
            std::vector<bool> localPart;
            localPart.resize(segmentSize(rankForRange), false);
//             std::queue<long> indexesOfFoundPrimes;
//             indexesOfFoundPrimes.insert(0);
            for(long i = 0; i < localPart.size(); ++i)
            {
                localPart[i] = true;
            }
            if(foremostWorker++ == rank)
            {
                std::string buffer;
                buffer = std::to_string(rank) + ": MPI_Bcast(&nextForemostWorker, 1, MPI_INT, " + std::to_string(rank) + ", MPI_COMM_WORLD);\n";
                std::cout << buffer << std::endl;
                broadcast(workersComm, foremostWorker, rank);
            }
        }
        std::string buffer = "Rank ";
        buffer += std::to_string(rank);
        buffer += " has beggining in ";
        buffer += std::to_string(segmentBegin(rankForRange));
        buffer += ", end in ";
        buffer += std::to_string(segmentEnd(rankForRange));
        buffer += ", size of ";
        buffer += std::to_string(segmentSize(rankForRange));
        buffer += '\n';
        std::cout << buffer;
    }
    return 0;
}

