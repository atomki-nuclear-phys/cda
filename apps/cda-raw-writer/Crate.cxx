// $Id$

// Qt include(s):
#include <QtCore/QtGlobal>
#include <QtCore/QFile>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/event/BinaryStream.h"
#else
#   include "event/BinaryStream.h"
#endif

// Local include(s):
#include "Crate.h"

namespace raw {

   Crate::Crate()
      : dev::Crate< dev::CernlibDisk >(),
        m_file( 0 ), m_stream( 0 ), m_evCounter( 0 ),
        m_logger( "raw::Crate" ) {

   }

   Crate::~Crate() {

      finalize();
   }

   bool Crate::initialize( const QString& fileName ) {

      // Check if a file is already open:
      if( m_file ) {
         REPORT_ERROR( tr( "Output file is already open" ) );
         return false;
      }

      //
      // Open a binary output file with this name:
      //
      m_file = new QFile( fileName );
      if( ! m_file->open( QFile::WriteOnly | QFile::Truncate ) ) {
         REPORT_ERROR( tr( "%1 couldn't be opened for writing" )
                       .arg( fileName ) );
         return false;
      } else {
         m_logger << msg::DEBUG
                  << tr( "%1 opened for writing" ).arg( fileName )
                  << msg::endmsg;
      }

      //
      // As a first thing write the configuration used for the
      // data taking to the file:
      //
      if( ! writeConfig( m_file ) ) {
         REPORT_ERROR( tr( "Couldn't write the configuration into the "
                           "output file" ) );
         return false;
      } else {
         m_logger << msg::DEBUG
                  << tr( "Configuration written into the output file" )
                  << msg::endmsg;
      }

      // Create the stream object on top of the file:
      if( m_stream ) {
         REPORT_ERROR( tr( "The object is in a very weird state. No file "
                           "was opened, but there was a stream created..." ) );
         return false;
      }
      m_stream = new ev::BinaryStream( m_file );

      // Reset the event counter:
      m_evCounter = 0;

      // Finally give some info to the user:
      m_logger << msg::INFO
               << tr( "%1 opened for writing" ).arg( fileName )
               << msg::endmsg;

      return true;
   }

   bool Crate::writeEvent( const ev::Event& event ) {

      // Check that the object has already been initialized:
      if( ( ! m_stream ) || ( ! m_file ) ) {
         REPORT_ERROR( tr( "Trying to write an event before opening an "
                           "output file" ) );
         return false;
      }

      // Simply write the event to the stream:
      *m_stream << event;

      // Flush the file on every 100th event:
      ++m_evCounter;
      if( m_evCounter >= 100 ) {
         if( ! m_file->flush() ) {
            REPORT_ERROR( tr( "There was an error flushing the "
                              "output file" ) );
            return false;
         }
         m_evCounter = 0;
      }

      return true;
   }

   bool Crate::finalize() {

      // First of all, let's delete the stream object:
      if( m_stream ) delete m_stream;
      m_stream = 0;

      // Next, flush the output file, then close it:
      if( m_file ) {
         if( ! m_file->flush() ) {
            REPORT_ERROR( tr( "There was an error flushing the output file "
                              "before closing it" ) );
            return false;
         }
         delete m_file;
      }
      m_file = 0;

      // Finally, let's reset the event counter:
      m_evCounter = 0;

      return true;
   }

} // namespace raw
