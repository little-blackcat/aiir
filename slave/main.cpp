#include <mpi.h>
#include <iostream>
#include <string>
#include <vector>

const int segmentSize(int rank, long endNum)
{
    return endNum / rank;
}

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int rank, numOfJobs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numOfJobs);

    if (rank == 0)
    {
        long value = std::stoi(argv[1]);
        int result = MPI_Bcast(&value, 1, MPI_LONG, 0, MPI_COMM_WORLD);
    }
    else if (rank > 0)
    {
        long value;
        int result = MPI_Bcast(&value, 1, MPI_LONG, 0, MPI_COMM_WORLD);
        
    }
    MPI_Finalize();
    return 0;
}
