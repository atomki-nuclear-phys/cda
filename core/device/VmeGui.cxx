// $Id$

// Local include(s):
#include "VmeGui.h"

namespace dev {

   // Specify the constant values:
   const int VmeGui::WIDGET_WIDTH  = 500;
   const int VmeGui::WIDGET_HEIGHT = 350;

   /**
    * The constructor fixes the size of the widget to the dimensions
    * specified by the WIDGET_WIDTH and WIDGET_HEIGHT constants.
    * Everything else is up to the derived classes...
    */
   VmeGui::VmeGui( QWidget* parent, Qt::WindowFlags flags )
      : QWidget( parent, flags ) {

      this->resize( WIDGET_WIDTH, WIDGET_HEIGHT );
      this->setMinimumSize( WIDGET_WIDTH, WIDGET_HEIGHT );
      this->setMaximumSize( WIDGET_WIDTH, WIDGET_HEIGHT );
   }

} // namespace dev
