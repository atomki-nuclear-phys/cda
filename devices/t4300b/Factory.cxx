// $Id$

// Qt include(s):
#include <QtCore/QtPlugin>

// Local include(s):
#include "Factory.h"
#include "Gui.h"
#include "Readout.h"
#include "CernlibHist.h"
#include "CernlibDisk.h"

namespace t4300b {

   QString Factory::shortName() const {

      return "T4300B";
   }

   QString Factory::longName() const {

      return "LeCroy 4300B FERA QDC";
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

} // namespace t4300b

// Declare this plugin to Qt:
Q_EXPORT_PLUGIN2( t4300b, t4300b::Factory )
