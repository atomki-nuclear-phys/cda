// $Id$

// Local include(s):
#include "CaenGui.h"

namespace dev {

   CaenGui::CaenGui( QWidget* parent, Qt::WindowFlags flags )
      : QWidget( parent, flags ) {

      this->resize( WIDGET_WIDTH, WIDGET_HEIGHT );
      this->setMinimumSize( WIDGET_WIDTH, WIDGET_HEIGHT );
      this->setMaximumSize( WIDGET_WIDTH, WIDGET_HEIGHT );
   }

} // namespace dev
