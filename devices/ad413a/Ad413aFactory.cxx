// $Id$

// Qt include(s):
#include <QtCore/QtPlugin>

// Local include(s):
#include "Ad413aFactory.h"
#include "Gui.h"
#include "Readout.h"
#include "CernlibHist.h"
#include "CernlibDisk.h"
#include "RootDisk.h"

namespace ad413a {

   QString Ad413aFactory::shortName() const {

      return "AD413A";
   }

   QString Ad413aFactory::longName() const {

      return tr( "ORTEC AD413A Quad 8k ADC" );
   }

   void* Ad413aFactory::createDevice( const std::type_info& ti ) const {

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

      }

      return 0;
   }

} // namespace ad413a

// Declare this plugin to Qt:
#if QT_VERSION < QT_VERSION_CHECK( 5, 0, 0 )
Q_EXPORT_PLUGIN2( ad413a, ad413a::Ad413aFactory )
#endif
