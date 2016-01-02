// $Id$

// Qt include(s):
#include <QtCore/QtPlugin>

// Local include(s):
#include "T2228aFactory.h"
#include "Gui.h"
#include "Readout.h"
#include "CernlibHist.h"
#include "CernlibDisk.h"
#include "RootDisk.h"

namespace t2228a {

   QString T2228aFactory::shortName() const {

      return "T2228A";
   }

   QString T2228aFactory::longName() const {

      return tr( "LeCroy 2228A TDC" );
   }

   void* T2228aFactory::createDevice( const std::type_info& ti ) const {

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

      }

      return 0;
   }

} // namespace t2228a

// Declare this plugin to Qt:
#if QT_VERSION < QT_VERSION_CHECK( 5, 0, 0 )
Q_EXPORT_PLUGIN2( t2228a, t2228a::T2228aFactory )
#endif
