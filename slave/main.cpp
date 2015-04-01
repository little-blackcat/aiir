#include <mpi.h>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <cmath>

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
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numOfJobs);
    problemSize = std::ceil(std::stol(argv[1]) / 2.0) - 1;

    int foremostWorker = 1;
    if (rank == 0)
    {
    }
    else if (rank > 0)
    {
        int rankForRange = rank - 1;
        std::vector<bool> localPart;
        localPart.resize(segmentSize(rankForRange), false);
        if(rank == foremostWorker)
        {

        }
        for(long i = 0; i < localPart.size(); ++i)
        {
            localPart[i] = true;
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
        std::cout << buffer << std::endl;
    }
    MPI_Finalize();
    return 0;
}
