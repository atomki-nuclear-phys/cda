// $Id$

// Qt include(s):
#include <QtCore/QtPlugin>

// Local include(s):
#include "Factory.h"
#include "Readout.h"

namespace s9418t {

   QString Factory::shortName() const {

      return "S9418T";
   }

   QString Factory::longName() const {

      return tr( "Silena 9418 TDC" );
   }

   void* Factory::createDevice( const std::type_info& ti ) const {

      if( ti == typeid( dev::VmeReadout ) ) {

         return dynamic_cast< dev::VmeReadout* >( new Readout() );

      }

      return 0;
   }

} // namespace s9418t

// Declare this plugin to Qt:
#if QT_VERSION < QT_VERSION_CHECK( 5, 0, 0 )
Q_EXPORT_PLUGIN2( s9418t, s9418t::Factory )
#endif
