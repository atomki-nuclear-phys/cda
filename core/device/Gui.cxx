// $Id$

// Local include(s):
#include "Gui.h"

namespace dev {

   //
   // Give values to the constants:
   //
   const int Gui::WIDGET_WIDTH  = 500;
   const int Gui::WIDGET_HEIGHT = 350;

   /**
    * The constructor sets the size of the widget in which the
    * device should draw. The widget size should not be changed
    * after this as the program will only provide a viewing area
    * WIDGET_WIDTH wide and WIDGET_HEIGHT high.
    */
   Gui::Gui( QWidget* parent, Qt::WindowFlags flags )
      : QWidget( parent, flags ) {

      this->resize( WIDGET_WIDTH, WIDGET_HEIGHT );
      this->setMinimumSize( WIDGET_WIDTH, WIDGET_HEIGHT );
      this->setMaximumSize( WIDGET_WIDTH, WIDGET_HEIGHT );

   }

   Gui::~Gui() {

   }

} // namespace dev