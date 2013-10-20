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

   QString Ad1000Factory::shortName() const {

      return "AD1000";
   }

   QString Ad1000Factory::longName() const {

      return tr( "Atomki AD1000 ADC" );
   }

   void* Ad1000Factory::createDevice( const std::type_info& ti ) const {

      if( ti == typeid( dev::CamacGui ) ) {

         return dynamic_cast< dev::CamacGui* >( new Gui() );

      } else if( ti == typeid( dev::CamacReadout ) ) {

         return dynamic_cast< dev::CamacReadout* >( new Readout() );

      } else if( ti == typeid( dev::CernlibHist ) ) {

         return dynamic_cast< dev::CernlibHist* >( new CernlibHist() );

      } else if( ti == typeid( dev::CernlibDisk ) ) {

         return dynamic_cast< dev::CernlibDisk* >( new CernlibDisk() );

      } else if( ti == typeid( dev::RootDisk ) ) {

         return dynamic_cast< dev::RootDisk* >( new RootDisk() );

      } else if( ti == typeid( dev::QtHist ) ) {

         return dynamic_cast< dev::QtHist* >( new QtHist() );

      }

      return 0;
   }

} // namespace ad1000

// Declare this plugin to Qt:
#if QT_VERSION < QT_VERSION_CHECK( 5, 0, 0 )
Q_EXPORT_PLUGIN2( ad1000, ad1000::Ad1000Factory )
#endif
