// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_CERNLIB_HISTMGR_H
#define CDA_CORE_CERNLIB_HISTMGR_H

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#include "../msg/Logger.h"

namespace cernlib {

   /**
    *  @short Interface used to handle monitoring histograms
    *
    *         This interface can be used to create and fill histograms
    *         in a PAW global memory segment. These histograms can then
    *         easily be monitored from inside PAW while the data acquisition
    *         is running.
    *
    *         It's not a full wrapper around the CERNLIB histogramming functions.
    *         I just created functions for doing the most basic things that
    *         CDA applications can need.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class HistMgr {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( cernlib::HistMgr )

   public:
      /// Constructor
      HistMgr();

      /// Initialise a PAW global memory segment
      void initialize( const char* gname = "CDA" );

      /// Book a 1-dimensional histogram
      int book_1d( const char* name, int channels,
                   double lowerBound, double upperBound );
      /// Book a 2-dimensional histogram
      int book_2d( const char* name, int xchannels,
                   double xlow, double xup, int ychannels,
                   double ylow, double yup );

      /// Fill a 1-dimensional histogram
      void fill_1d( int id, double data, double weight = 1.0 ) const;
      /// Fill a 2-dimensional histogram
      void fill_2d( int id, double xdata, double ydata,
                    double weight = 1.0 ) const;

   private:
      /// Function used internally to check if the global memory is initialised
      bool checkInit() const;

      /// Variable showing if the global memory is initialised
      bool m_initialized;
      int m_counter; ///< Variable keeping track of available IDs
      mutable msg::Logger m_logger; ///< Message logging object

   }; // class HistMgr

} // namespace cernlib

#endif // CDA_CORE_CERNLIB_HISTMGR_H
