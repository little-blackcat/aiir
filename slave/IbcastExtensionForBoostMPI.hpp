#include <boost/mpi/collectives/broadcast.hpp>
#include <boost/mpi/request.hpp>

#pragma once

namespace boost{
namespace mpi{
namespace detail {
  // We're sending a type that has an associated MPI datatype, so
  // we'll use MPI_Ibcast to do all of the work.
  template<typename T>
  request
  ibroadcast_impl(const communicator& comm, T* values, int n, int root,
                 mpl::true_)
  {
    request req;
    BOOST_MPI_CHECK_RESULT(MPI_Ibcast,
                           (values, n,
                            boost::mpi::get_mpi_datatype<T>(*values),
                            root,
                            MPI_Comm(comm),
                            &req.m_requests[0]));
    return req;
  }

  // Fail badly if there isn't any associated datatype, there is no implementation here
  template<typename T>
  void
  ibroadcast_impl(const communicator& comm, T* values, int n, int root,
                 mpl::false_)
  {
    throw_exception(boost::mpi::exception("MPI_Ibcast", "wrong datatype"));
  }
} // end namespace detail

  template<typename T>
  request ibroadcast(const communicator& comm, T& value, int root)
  {
      return detail::ibroadcast_impl(comm, &value, 1, root, is_mpi_datatype<T>());
  }

  template<typename T>
  request ibroadcast(const communicator& comm, T* values, int n, int root)
  {
      return detail::ibroadcast_impl(comm, values, n, root, is_mpi_datatype<T>());
  }
}} // end namespace boost::mpi
