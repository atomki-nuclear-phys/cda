// $Id$

// Qt include(s):
#include <QtCore/QtPlugin>

// Local include(s):
#include "S9418tFactory.h"
#include "Readout.h"

namespace s9418t {

   const QString& S9418tFactory::shortName() const {

      static const QString name( "S9418T" );
      return name;
   }

   const QString& S9418tFactory::longName() const {

      static const QString name( tr( "Silena 9418 TDC" ) );
      return name;
   }

   void* S9418tFactory::createDevice( const std::type_info& ti ) const {

      if( ti == typeid( dev::IVmeReadout ) ) {

         return static_cast< dev::IVmeReadout* >( new Readout() );

      }

      return 0;
   }

} // namespace s9418t

// Declare this plugin to Qt:
#if QT_VERSION < QT_VERSION_CHECK( 5, 0, 0 )
Q_EXPORT_PLUGIN2( s9418t, s9418t::S9418tFactory )
#endif
