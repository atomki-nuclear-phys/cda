// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_CERNLIB_HISTMGR_H
#define CDA_CORE_CERNLIB_HISTMGR_H

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#include "../msg/Logger.h"

namespace cernlib {

   class HistMgr {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( cernlib::HistMgr )

   public:
      HistMgr();

      void initialize( const char* gname = "CDA" );

      int book_1d( const char* name, int channels,
                   double lowerBound, double upperBound );
      int book_2d( const char* name, int xchannels,
                   double xlow, double xup, int ychannels,
                   double ylow, double yup );

      void fill_1d( int id, double data, double weight ) const;
      void fill_2d( int id, double xdata, double ydata,
                    double weight ) const;

   private:
      bool checkInit() const;

      /// Variable showing if the global memory is initialised
      bool m_initialized;
      int m_counter; ///< Variable keeping track of available IDs
      mutable msg::Logger m_logger; ///< Message logging object

   }; // class HistMgr

} // namespace cernlib

#endif // CDA_CORE_CERNLIB_HISTMGR_H
