// $Id$

// Qt include(s):
#include <QtCore/QtPlugin>

// Local include(s):
#include "Dt5740Factory.h"
#include "Gui.h"
#include "CernlibHist.h"
#include "CernlibDisk.h"
#include "Readout.h"
#include "RootDisk.h"
#include "QtHist.h"

namespace dt5740 {

   QString Dt5740Factory::shortName() const {

      return "DT5740";
   }

   QString Dt5740Factory::longName() const {

      return tr( "CAEN DT5740 Desktop Digitizer" );
   }

   void* Dt5740Factory::createDevice( const std::type_info& ti ) const {

      if( ti == typeid( dev::CaenGui ) ) {

         return dynamic_cast< dev::CaenGui* >( new Gui() );

      } else if( ti == typeid( dev::CernlibHist ) ) {

         return dynamic_cast< dev::CernlibHist* >( new CernlibHist() );

      } else if( ti == typeid( dev::CernlibDisk ) ) {

         return dynamic_cast< dev::CernlibDisk* >( new CernlibDisk() );

      } else if( ti == typeid( dev::CaenReadout ) ) {

         return dynamic_cast< dev::CaenReadout* >( new Readout() );

      } else if( ti == typeid( dev::RootDisk ) ) {

         return dynamic_cast< dev::RootDisk* >( new RootDisk() );

      } else if( ti == typeid( dev::QtHist ) ) {

         return dynamic_cast< dev::QtHist* >( new QtHist() );

      }

      return 0;
   }

} // namespace dt5740

// Declare this plugin to Qt:
#if QT_VERSION < QT_VERSION_CHECK( 5, 0, 0 )
Q_EXPORT_PLUGIN2( dt5740, dt5740::Dt5740Factory )
#endif
