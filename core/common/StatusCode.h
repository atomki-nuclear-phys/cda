// Dear emacs, this is -*- c++ -*-
#ifndef CDA_CORE_COMMON_STATUSCODE_H
#define CDA_CORE_COMMON_STATUSCODE_H

// Local include(s):
#include "Export.h"

/**
 * @short Type used as the return type of functions that can fail
 *
 * This class is taken pretty much directly from the ATLAS offline/analysis
 * code. Used here to provide "safer" error detection and propagation in the
 * code.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */
class CDACORE_EXPORT StatusCode {

public:
   /// Convenience StatusCode types
   enum {
      FAILURE = 0,
      SUCCESS = 1
   };

   /// Constructor from an integer status code
   StatusCode( unsigned long rstat = SUCCESS );
   /// Copy constructor
   StatusCode( const StatusCode& parent );
   /// Destructor
   ~StatusCode();

   /// Assignment operator
   StatusCode& operator= ( const StatusCode& rhs );
   /// Assignment from an integer code
   StatusCode& operator= ( unsigned long code );

   /// Check if the operation was successful
   bool isSuccess() const;
   /// Check if the operation was a failure
   bool isFailure() const;

   /// Automatic conversion operator
   operator unsigned long() const;

   /// Mark the status code as checked, ignoring it thereby
   void setChecked() const { m_checked = true; }
   /// Ignore the status code, marking it as checked
   void ignore() const { setChecked(); }

   /// Enable failure (with a backtrace) on an unchecked status code
   static void enableFailure();
   /// Disable failure (no backtrace) on an unchecked status code
   static void disableFailure();

private:
   /// Code returned by some function
   unsigned long m_code;
   /// Internal status flag of whether the code was checked by the user
   mutable bool m_checked;

}; // class StatusCode

#endif // CDA_CORE_COMMON_STATUSCODE_H
