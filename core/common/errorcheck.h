// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_COMMON_ERRORCHECK_H
#define CDA_CORE_COMMON_ERRORCHECK_H

// Qt include(s):
#include <QtCore/QCoreApplication>

/// Macro for handling functions returning boolean status values
/**
 * In many places of the code functions return boolean values, and
 * show their success or failure with that simple boolean return
 * value. To avoid having to write the same
 *
 * <code>
 * if( ! functionCall() ) {
 *    REPORT_ERROR( tr( "Bla" ) );
 *    return false;
 * }
 * </code>
 *
 * code over and over again, this function can be used. It prints
 * a good-enough error message when something goes wrong, while
 * it is very short to write... One should always use it like:
 *
 * <code>
 * CHECK( functionCall() );
 * </code>
 */
#define CHECK( CMD ) {                                               \
      const bool result = CMD;                                       \
      if( ! result ) {                                               \
         REPORT_ERROR( qApp->translate( "CHECK",                     \
                                        "Failed executing \"%1\"" )  \
                       .arg( #CMD ) );                               \
         return result;                                              \
      }                                                              \
   } while( 0 ){}

#endif // CDA_CORE_COMMON_ERRORCHECK_H
