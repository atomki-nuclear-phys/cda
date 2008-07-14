// $Id$

// CERNLIB include(s):
extern "C" {
#   include <cfortran/cfortran.h>
#   include <cfortran/hbook.h>
}

// Local include(s):
#include "HistMgr.h"

/// Size of the PAW global memory
#define PAWC_SIZE 100000

namespace cernlib {

   HistMgr::HistMgr()
      : m_initialized( false ), m_counter( 0 ),
        m_logger( "cernlib::HistMgr" ) {

   }

   void HistMgr::initialize( const char* gname ) {

      m_counter = 0;

      if( ! m_initialized ) {
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

   void HistMgr::fill_1d( int id, double data, double weight ) const {

      // Check that the global memory is available:
      if( ! checkInit() ) return;

      // Fill the histogram:
      HF1( id, data, weight );

      return;

   }

   void HistMgr::fill_2d( int id, double xdata, double ydata,
                          double weight ) const {

      // Check that the global memory is available:
      if( ! checkInit() ) return;

      // Fill the histogram:
      HF2( id, xdata, ydata, weight );

      return;

   }

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
