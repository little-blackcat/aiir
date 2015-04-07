#include <iostream>
#include <boost/mpi.hpp>
#include "Worker.hpp"

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
    auto worker = Worker::getInstance(problemSize, workersComm);
    worker->run();
    return 0;
}
