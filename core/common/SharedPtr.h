// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_COMMON_SHAREDPTR_H
#define CDA_CORE_COMMON_SHAREDPTR_H

// System include(s):
#if __cplusplus < 201100
#   include <tr1/memory>
#else
#   include <memory>
#endif // c++11

/**
 * @short Traits class defining a shared pointer type
 *
 *        In order to be able to use adequate smart shared pointer types
 *        both in c++98 and c++11 mode, this traits class is used everywhere
 *        in the code to declare shared pointer types.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 *
 * $Revision$
 * $Date$
 */
template< class T >
struct SharedPtr {

#if __cplusplus < 201100
   typedef std::tr1::shared_ptr< T > Type;
#else
   typedef std::shared_ptr< T > Type;
#endif // c++11

}; // struct SharedPtr

#endif // CDA_CORE_COMMON_SHAREDPTR_H
