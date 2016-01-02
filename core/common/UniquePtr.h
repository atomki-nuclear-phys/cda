// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_COMMON_UNIQUEPTR_H
#define CDA_CORE_COMMON_UNIQUEPTR_H

// System include(s):
#include <memory>

/**
 * @short Traits class defining a unique pointer type
 *
 *        In order to be able to use adequate smart unique pointer types
 *        both in c++98 and c++11 mode, this traits class is used everywhere
 *        in the code to declare unique pointer types.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 *
 * $Revision$
 * $Date$
 */
template< class T >
struct UniquePtr {

   /// The type used as a unique pointer
#if __cplusplus < 201100
   typedef std::auto_ptr< T > Type;
#else
   typedef std::unique_ptr< T > Type;
#endif // c++11

   /// Function swapping the payload of two unique pointer objects
   static void swap( Type& obj1, Type& obj2 ) {
#if __cplusplus < 201100
      // Cache the value of obj1 for now:
      Type tmp( obj1 );
      // Now swap the two:
      obj1 = obj2;
      obj2 = tmp;
#else
      // Use the STL swap function:
      std::swap( obj1, obj2 );
#endif // c++11
   }

}; // struct UniquePtr

/// Helper macro for using std::move in c++11 mode
#if __cplusplus < 201100
#   define UNIQUE_PTR_MOVE( X ) X
#else
#   define UNIQUE_PTR_MOVE( X ) std::move( X )
#endif // c++11

#endif // CDA_CORE_COMMON_UNIQUEPTR_H
