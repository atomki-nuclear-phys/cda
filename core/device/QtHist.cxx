// $Id$

// Local include(s):
#include "QtHist.h"

namespace dev {

   QtHist::QtHist( QWidget* parent, Qt::WindowFlags flags )
      : QWidget( parent, flags ) {

   }

   QString QtHist::windowTitle() const {

      return ( tr( "%1 with ID %2" )
               .arg( this->deviceName() )
               .arg( this->getID() ) );
   }

} // namespace dev
