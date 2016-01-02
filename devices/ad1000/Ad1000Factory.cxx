// $Id$

// Qt include(s):
#include <QtCore/QtPlugin>

// Local include(s):
#include "Ad1000Factory.h"
#include "Gui.h"
#include "Readout.h"
#include "CernlibHist.h"
#include "CernlibDisk.h"
#include "RootDisk.h"
#include "QtHist.h"

namespace ad1000 {

   const QString& Ad1000Factory::shortName() const {

      static const QString name( "AD1000" );
      return name;
   }

   const QString& Ad1000Factory::longName() const {

      static const QString name( tr( "Atomki AD1000 ADC" ) );
      return name;
   }

   void* Ad1000Factory::createDevice( const std::type_info& ti ) const {

      if( ti == typeid( dev::CamacGui ) ) {

         return static_cast< dev::CamacGui* >( new Gui() );

      } else if( ti == typeid( dev::ICamacReadout ) ) {

         return static_cast< dev::ICamacReadout* >( new Readout() );

      } else if( ti == typeid( dev::ICernlibHist ) ) {

         return static_cast< dev::ICernlibHist* >( new CernlibHist() );

      } else if( ti == typeid( dev::ICernlibDisk ) ) {

         return static_cast< dev::ICernlibDisk* >( new CernlibDisk() );

      } else if( ti == typeid( dev::IRootDisk ) ) {

         return static_cast< dev::IRootDisk* >( new RootDisk() );

      } else if( ti == typeid( dev::QtHist ) ) {

         return static_cast< dev::QtHist* >( new QtHist() );

      }

      return 0;
   }

} // namespace ad1000

// Declare this plugin to Qt:
#if QT_VERSION < QT_VERSION_CHECK( 5, 0, 0 )
Q_EXPORT_PLUGIN2( ad1000, ad1000::Ad1000Factory )
#endif
