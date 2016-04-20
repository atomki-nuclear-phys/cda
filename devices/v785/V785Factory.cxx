// $Id$

// Qt include(s):
#include <QtCore/QtPlugin>

// Local include(s):
#include "V785Factory.h"
#include "CernlibHist.h"
#include "CernlibDisk.h"
#include "Gui.h"
#include "Readout.h"
#include "RootDisk.h"
#include "QtHist.h"

namespace v785 {

   const QString& V785Factory::shortName() const {

      static const QString name( "V785" );
      return name;
   }

   const QString& V785Factory::longName() const {

      static const QString name( tr( "CAEN V785 ADC" ) );
      return name;
   }

   void* V785Factory::createDevice( const std::type_info& ti ) const {

      if( ti == typeid( dev::ICaenVmeReadout ) ) {

         return static_cast< dev::ICaenVmeReadout* >( new Readout() );

      } else if( ti == typeid( dev::CaenVmeGui ) ) {

         return static_cast< dev::CaenVmeGui* >( new Gui() );

      } else if( ti == typeid( dev::IRootDisk ) ) {

         return static_cast< dev::IRootDisk* >( new RootDisk() );

      } else if( ti == typeid( dev::QtHist ) ) {

         return static_cast< dev::QtHist* >( new QtHist() );

      } else if( ti == typeid( dev::ICernlibHist ) ) {

         return static_cast< dev::ICernlibHist* >( new CernlibHist() );

      } else if( ti == typeid( dev::ICernlibDisk ) ) {

         return static_cast< dev::ICernlibDisk* >( new CernlibDisk() );

      }

      return 0;
   }

} // namespace v785

// Declare this plugin to Qt:
#if QT_VERSION < QT_VERSION_CHECK( 5, 0, 0 )
Q_EXPORT_PLUGIN2( v785, v785::V785Factory )
#endif
