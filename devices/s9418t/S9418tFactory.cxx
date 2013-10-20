// $Id$

// Qt include(s):
#include <QtCore/QtPlugin>

// Local include(s):
#include "S9418tFactory.h"
#include "Readout.h"

namespace s9418t {

   QString S9418tFactory::shortName() const {

      return "S9418T";
   }

   QString S9418tFactory::longName() const {

      return tr( "Silena 9418 TDC" );
   }

   void* S9418tFactory::createDevice( const std::type_info& ti ) const {

      if( ti == typeid( dev::VmeReadout ) ) {

         return dynamic_cast< dev::VmeReadout* >( new Readout() );

      }

      return 0;
   }

} // namespace s9418t

// Declare this plugin to Qt:
#if QT_VERSION < QT_VERSION_CHECK( 5, 0, 0 )
Q_EXPORT_PLUGIN2( s9418t, s9418t::S9418tFactory )
#endif
