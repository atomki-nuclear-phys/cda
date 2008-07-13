// $Id$

// CDA include(s):
#include "msg/Logger.h"

// Local include(s):
#include "Fifo.h"

/**
 * @param path The name of the FIFO file
 * @param parent The Qt parent of the object
 */
Fifo::Fifo( const QString& path, QObject* parent )
   : QT_PREPEND_NAMESPACE( QFile )( path, parent ) {

}

Fifo::~Fifo() {

   close();

}

/**
 * This function should always be used to open FIFO files. It makes
 * sure that the requested FIFO exists before trying to open it.
 * Since we always open FIFOs in <code>QIODevice::ReadWrite</code>
 * mode, a missing FIFO would otherwise be silentry created as a regular
 * file.
 *
 * @returns <code>true</code> if the operation was successful,
 *          <code>false</code> otherwise
 */
bool Fifo::open() {

   msg::Logger logger( "Fifo::open()" );
   if( exists() ) {
      logger << msg::VERBOSE << tr( "%1 exists" ).arg( fileName() )
             << msg::endmsg;
      return QT_PREPEND_NAMESPACE( QFile )::open( QIODevice::ReadWrite );
   } else {
      logger << msg::WARNING << tr( "%1 does not exist" ).arg( fileName() )
             << msg::endmsg;
      return false;
   }

}
