// $Id$

// Qt include(s):
#include <QtCore/QtPlugin>

// Local include(s):
#include "Factory.h"
#include "Gui.h"

namespace dt5740 {

   QString Factory::shortName() const {

      return "DT5740";
   }

   QString Factory::longName() const {

      return "CAEN DT5740 Desktop Digitizer";
   }

   void* Factory::createDevice( const std::type_info& ti ) const {

      if( ti == typeid( dev::CaenGui ) ) {

         return dynamic_cast< dev::CaenGui* >( new Gui() );

      }

      return 0;
   }

} // namespace dt5740

// Declare this plugin to Qt:
Q_EXPORT_PLUGIN2( dt5740, dt5740::Factory )
