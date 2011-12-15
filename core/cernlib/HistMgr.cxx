// $Id$

// CERNLIB include(s):
#ifdef HAVE_CERNLIB
extern "C" {
#   include <cfortran/cfortran.h>
#   include <hbook.h>
}
#endif // HAVE_CERNLIB

// Local include(s):
#include "HistMgr.h"

/// Size of the PAW global memory
#define PAWC_SIZE 100000

namespace cernlib {

   /**
    * The constructor doesn't do anything fancy, it just initialises all the
    * member variables to sensible defaults.
    */
   HistMgr::HistMgr()
      : m_initialized( false ), m_counter( 0 ),
        m_logger( "cernlib::HistMgr" ) {

      REPORT_VERBOSE( tr( "Object constructed" ) );
   }

   /**
    * This function initialises a PAW global memory segment to create
    * histograms in. Note that the name of the global memory segment should
    * not be longer than 4 characters! Leaving it at its default value
    * is usually a good idea...
    *
    * @param gname Global memory name (maximum 4 characters)
    */
   void HistMgr::initialize( const char* gname ) {

      // Return right away if CERNLIB is not available:
#ifndef HAVE_CERNLIB
      m_logger << msg::DEBUG
               << tr( "Booking global memory block: %1" ).arg( gname )
               << msg::endmsg;
      return;
#endif // HAVE_CERNLIB

      m_counter = 0;

      if( ! m_initialized ) {

         // CERNLIB is really weird. It doesn't like constant pointers as
         // character strings...
         char* name = const_cast< char* >( gname );
         if( ! name ) {
            REPORT_ERROR( tr( "Couldn't const_cast specified global "
                              "memory name" ) );
            return;
         }
#ifdef HAVE_CERNLIB
         HLIMAP( PAWC_SIZE, name );
#endif // HAVE_CERNLIB
         m_logger << msg::DEBUG
                  << tr( "Created PAW global memory with name: \"%1\"" ).arg( name )
                  << msg::endmsg;
         m_initialized = true;

      }

      return;
   }

   /**
    * This function creates a new histogram in the PAW global memory with
    * the next available ID.
    *
    * @param name Name of the histogram (can be reasonably long)
    * @param channels Channel count in the histogram
    * @param lowerBound The lower edge of the histogram
    * @param upperBound The upper edge of the histogram
    * @returns The ID with which the histogram was created
    */
   int HistMgr::book_1d( const char* name, int channels,
                         double lowerBound, double upperBound ) {

      // Return right away if CERNLIB is not available:
#ifndef HAVE_CERNLIB
      REPORT_VERBOSE( tr( "Booking 1D histogram with name \"%1\", channels: %2, "
                          "lower bound: %3, upper bound: %4" )
                      .arg( name ).arg( channels ).arg( lowerBound )
                      .arg( upperBound ) );
      return -1;
#endif // HAVE_CERNLIB

      // Check that the global memory is available:
      if( ! checkInit() ) return -1;

      // Do this ugly cast to make CERNLIB happy:
      char* lname = const_cast< char* >( name );
      if( ! lname ) {
         REPORT_ERROR( tr( "Couldn't const_cast specified histogram name" ) );
         return -1;
      }

      // Increment the histogram counter:
      ++m_counter;

      // Book the histogram:
#ifdef HAVE_CERNLIB
      HBOOK1( m_counter, lname, channels, lowerBound, upperBound, 0. );
#endif // HAVE_CERNLIB

      // Return the ID of the histogram:
      return m_counter;
   }

   /**
    * This function creates a new histogram in the PAW global memory with
    * the next available ID.
    *
    * @param name Name of the histogram (can be reasonably long)
    * @param xchannels Channel count on the X axis
    * @param xlow Lower edge of the X axis
    * @param xup Upper edge of the X axis
    * @param ychannels Channel count on the Y axis
    * @param ylow Lower edge of the Y axis
    * @param yup Upper edge of the Y axis
    * @returns The ID with which the histogram was created
    */
   int HistMgr::book_2d( const char* name, int xchannels,
                         double xlow, double xup, int ychannels,
                         double ylow, double yup ) {

      // Return right away if CERNLIB is not available:
#ifndef HAVE_CERNLIB
      REPORT_VERBOSE( tr( "Booking 2D histogram with name \"%1\", x-channels: %2, "
                          "x-low: %3, x-up: %4, y-channels: %5, y-low: %6, y-up: %7" )
                      .arg( name ).arg( xchannels ).arg( xlow ).arg( xup )
                      .arg( ychannels ).arg( ylow ).arg( yup ) );
      return -1;
#endif // HAVE_CERNLIB

      // Check that the global memory is available:
      if( ! checkInit() ) return -1;

      // Do this ugly cast to make CERNLIB happy:
      char* lname = const_cast< char* >( name );
      if( ! lname ) {
         REPORT_ERROR( tr( "Couldn't const_cast specified histogram name" ) );
         return -1;
      }

      // Increment the histogram counter:
      ++m_counter;

      // Book the histogram:
#ifdef HAVE_CERNLIB
      HBOOK2( m_counter, lname, xchannels, xlow, xup, ychannels,
              ylow, yup, 0. );
#endif // HAVE_CERNLIB

      // Return the ID of the histogram:
      return m_counter;
   }

   /**
    * This function adds one entry to the specified 1-dimensional
    * histogram. The ID has to be the same that the HistMgr::book_1d
    * function returned.
    *
    * @param id The ID of the histogram to fill
    * @param data The value that has to be filled
    * @param weight The weight of the event
    */
   void HistMgr::fill_1d( int id, double data, double weight ) const {

      // Return right away if CERNLIB is not available:
#ifndef HAVE_CERNLIB
      REPORT_VERBOSE( tr( "Filling histogram %1 with data: %2, weight: %3" )
                      .arg( id ).arg( data ).arg( weight ) );
      return;
#endif // HAVE_CERNLIB

      // Check that the global memory is available:
      if( ! checkInit() ) return;

      // Fill the histogram:
#ifdef HAVE_CERNLIB
      HF1( id, data, weight );
#endif // HAVE_CERNLIB

      return;
   }

   /**
    * This function adds one entry to the specified 2-dimensional
    * histogram. The ID has to be the same that the HistMgr::book_2d
    * function returned.
    *
    * @param id The ID of the histogram to fill
    * @param xdata The X value of the entry
    * @param ydata The Y value of the entry
    * @param weight The weight of the event
    */
   void HistMgr::fill_2d( int id, double xdata, double ydata,
                          double weight ) const {

      // Return right away if CERNLIB is not available:
#ifndef HAVE_CERNLIB
      REPORT_VERBOSE( tr( "Filling histogram %1 with xdata: %2, ydata: %3, "
                          "weight: %4" )
                      .arg( id ).arg( xdata ).arg( ydata ).arg( weight ) );
      return;
#endif // HAVE_CERNLIB

      // Check that the global memory is available:
      if( ! checkInit() ) return;

      // Fill the histogram:
#ifdef HAVE_CERNLIB
      HF2( id, xdata, ydata, weight );
#endif // HAVE_CERNLIB

      return;
   }

   /**
    * To minimise the amound of code a bit, this function is called by all
    * the member functions that need the PAW global memory to be initialised.
    *
    * @returns <code>true</code> If the memory is initialised,
    *          <code>false</code> otherwise
    */
   bool HistMgr::checkInit() const {

      if( ! m_initialized ) {
         REPORT_ERROR( tr( "Trying to create/access histograms without\n"
                           "initializing the global memory first" ) );
         return false;
      }

      return true;
   }

} // namespace cernlib
