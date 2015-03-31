#include <mpi.h>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0)
    {
        int value = std::stoi(argv[1]);
        int result = MPI_Bcast(&value, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if (result == MPI_SUCCESS)
            std::cout << "Rank 0 OK!" << std::endl;
        else
            std::cout << "Rank 0 blew up!" << std::endl;
    } else if (rank > 0)
    {
        int value;
        int result = MPI_Bcast(&value, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if (result == MPI_SUCCESS && value == 17)
            std::cout << "Rank " << rank << " OK!" << std::endl;
        else
        {
            std::cout << "Rank " << rank << " blew up!" << std::endl;
            return 0;
        }
    }
    MPI_Finalize();
    return 0;
}
