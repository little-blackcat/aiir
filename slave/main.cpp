#include <iostream>
#include <boost/mpi.hpp>
#include "Worker.hpp"



int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cerr << "Binary takes one and only one additional argument: size of range to seek for primes" << std::endl;
        return -1;
    }
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;
    const long problemSize = std::ceil(std::stol(argv[1]) / 2.0) - 1;
    auto workersComm = world;
    auto worker = Worker::getInstance(problemSize, workersComm);
    worker->run();
    long sumOfPrimes = 0;
    long output = 0;
    boost::mpi::reduce(world, sumOfPrimes, output, std::plus<>(), 0);
    std::cout << output << std::endl;
    return 0;
}
