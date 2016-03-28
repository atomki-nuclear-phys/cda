// $Id$

// Local include(s):
#include "CaenVmeBusWidget.h"

namespace dev {

   CaenVmeBusWidget::CaenVmeBusWidget( QWidget* parent,
                                       Qt::WindowFlags flags )
      : QWidget( parent, flags ) {

      // Set the size of the widget:
      setMinimumSize( WIDGET_WIDTH, WIDGET_HEIGHT );
      setMaximumSize( WIDGET_WIDTH, WIDGET_HEIGHT );
   }

} // namespace dev
