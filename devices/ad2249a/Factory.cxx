
// Qt include(s):
#include <QtCore/QtPlugin>

// Local include(s):
#include "Factory.h"
#include "Gui.h"
#include "Readout.h"
#include "CernlibHist.h"
#include "CernlibDisk.h"

namespace ad2249a {

   QString Factory::shortName() const {

      return "AD2249A";
   }

   QString Factory::longName() const {

      return "LeCroy 2249A ADC";
   }

   void* Factory::createDevice( const std::type_info& ti ) const {

      if( ti == typeid( dev::CamacGui ) ) {

         return dynamic_cast< dev::CamacGui* >( new Gui() );

      } else if( ti == typeid( dev::CamacReadout ) ) {

         return dynamic_cast< dev::CamacReadout* >( new Readout() );

      } else if( ti == typeid( dev::CernlibHist ) ) {

         return dynamic_cast< dev::CernlibHist* >( new CernlibHist() );

      } else if( ti == typeid( dev::CernlibDisk ) ) {

         return dynamic_cast< dev::CernlibDisk* >( new CernlibDisk() );

      }

      return 0;
   }

} // namespace ad2249a

// Declare this plugin to Qt:
Q_EXPORT_PLUGIN2( ad2249a, ad2249a::Factory )
