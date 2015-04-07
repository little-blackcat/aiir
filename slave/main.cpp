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

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cerr << "Binary takes one and only one additional argument: size of range to seek for primes" << std::endl;
        return -1;
    }
    mpi::environment env(argc, argv, mpi::threading::multiple);
    mpi::communicator world;
    const long problemSize = std::ceil(std::stol(argv[1]) / 2.0) - 1;
    auto workersComm = world;
//     const int globalRank = world.rank();
//     boost::mpi::communicator workersComm = world.split(globalRank && true);
//
//     std::string buffer = "workersComm.size " + std::to_string(workersComm.size()) + '\n';
//     std::cout << buffer << std::endl;
//     int foremostWorker = 0;
//     if (globalRank == 0)
//     {
//         foremostWorker = 1;
//         mpi::request req[1] = {mpi::ibroadcast(world, foremostWorker, 0)};
//         mpi::wait_all(req, req + 1);
//     }
//     else if (globalRank > 0)
//     {
        auto worker = Worker::getInstance(problemSize, workersComm);
//         mpi::request req[1] = {mpi::ibroadcast(world, foremostWorker, 0)};
        worker->run();
//     }
    return 0;
}

