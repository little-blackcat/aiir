#include <mpi.h>
#include <iostream>
#include <string>
#include <vector>

inline const long segmentBegin(int rank, int numOfJobs, long size)
{
    return rank * size / numOfJobs;
}
inline const long segmentEnd(int rank, int numOfJobs, long size)
{
    return segmentBegin(rank + 1, numOfJobs, size);
}
inline const long segmentSize(int rank, int numOfJobs, long size)
{
    return segmentEnd(rank, numOfJobs, size) - segmentBegin(rank, numOfJobs, size);
}

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int rank, numOfJobs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numOfJobs);
    if (rank == 0)
    {
        long value = std::stol(argv[1]);
        int result = MPI_Bcast(&value, 1, MPI_LONG, 0, MPI_COMM_WORLD);
    }
    else if (rank > 0)
    {
        int rankForRange = rank - 1;
        long value;
        int result = MPI_Bcast(&value, 1, MPI_LONG, 0, MPI_COMM_WORLD);
        std::vector<bool> localPart;
        localPart.resize(segmentSize(rankForRange, numOfJobs, value), false);
        localPart[segmentEnd(rankForRange, numOfJobs, value) - 1] = true;
        std::string buffer = "Rank ";
        buffer += std::to_string(rank);
        buffer += " has beggining in ";
        buffer += std::to_string(segmentBegin(rankForRange, numOfJobs, value));
        buffer += ", end in ";
        buffer += std::to_string(segmentEnd(rankForRange, numOfJobs, value));
        buffer += ", size of ";
        buffer += std::to_string(segmentSize(rankForRange, numOfJobs, value));
        buffer += "\n";
        std::cout << buffer << std::endl;
    }
    MPI_Finalize();
    return 0;
}
