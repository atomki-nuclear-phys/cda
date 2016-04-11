// $Id$

// Qt include(s):
#include <QtCore/QtPlugin>

// Local include(s):
#include "V862Factory.h"
#include "Gui.h"
#include "Readout.h"
#include "RootDisk.h"

namespace v862 {

   const QString& V862Factory::shortName() const {

      static const QString name( "V862" );
      return name;
   }

   const QString& V862Factory::longName() const {

      static const QString name( tr( "CAEN V862 QDC" ) );
      return name;
   }

   void* V862Factory::createDevice( const std::type_info& ti ) const {

      if( ti == typeid( dev::ICaenVmeReadout ) ) {

         return static_cast< dev::ICaenVmeReadout* >( new Readout() );

      } else if( ti == typeid( dev::CaenVmeGui ) ) {

         return static_cast< dev::CaenVmeGui* >( new Gui() );

      } else if( ti == typeid( dev::IRootDisk ) ) {

         return static_cast< dev::IRootDisk* >( new RootDisk() );

      }

      return 0;
   }

} // namespace t2228a

// Declare this plugin to Qt:
#if QT_VERSION < QT_VERSION_CHECK( 5, 0, 0 )
Q_EXPORT_PLUGIN2( v862, v862::V862Factory )
#endif
