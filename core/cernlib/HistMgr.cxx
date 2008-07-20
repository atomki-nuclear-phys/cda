// $Id$

// CERNLIB include(s):
extern "C" {
#   include <cfortran/cfortran.h>
#   include <hbook.h>
}

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

      m_counter = 0;

      if( ! m_initialized ) {

         // CERNLIB is really weird. It doesn't like constant pointers as
         // character strings...
         char* name = const_cast< char* >( gname );
         if( ! name ) {
            m_logger << msg::ERROR
                     << tr( "Couldn't const_cast specified global memory name" )
                     << msg::endmsg;
            return;
         }
         HLIMAP( PAWC_SIZE, name );
         m_logger << msg::DEBUG
                  << tr( "Created PAW global memory with name: \"%1\"" )
            .arg( name )
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

      // Check that the global memory is available:
      if( ! checkInit() ) return -1;

      // Do this ugly cast to make CERNLIB happy:
      char* lname = const_cast< char* >( name );
      if( ! lname ) {
         m_logger << msg::ERROR
                  << tr( "Couldn't const_cast specified histogram name" )
                  << msg::endmsg;
         return -1;
      }

      // Increment the histogram counter:
      ++m_counter;

      // Book the histogram:
      HBOOK1( m_counter, lname, channels, lowerBound, upperBound, 0. );

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

      // Check that the global memory is available:
      if( ! checkInit() ) return -1;

      // Do this ugly cast to make CERNLIB happy:
      char* lname = const_cast< char* >( name );
      if( ! lname ) {
         m_logger << msg::ERROR
                  << tr( "Couldn't const_cast specified histogram name" )
                  << msg::endmsg;
         return -1;
      }

      // Increment the histogram counter:
      ++m_counter;

      // Book the histogram:
      HBOOK2( m_counter, lname, xchannels, xlow, xup, ychannels,
              ylow, yup, 0. );

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

      // Check that the global memory is available:
      if( ! checkInit() ) return;

      // Fill the histogram:
      HF1( id, data, weight );

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

      // Check that the global memory is available:
      if( ! checkInit() ) return;

      // Fill the histogram:
      HF2( id, xdata, ydata, weight );

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
         m_logger << msg::ERROR
                  << tr( "Trying to create/access histograms without\n"
                         "initializing the global memory first" )
                  << msg::endmsg;
         return false;
      }

      return true;

   }

} // namespace cernlib
