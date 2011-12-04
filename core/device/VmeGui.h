// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_DEVICES_VMEGUI_H
#define CDA_CORE_DEVICES_VMEGUI_H

// Qt include(s):
#include <QtGui/QWidget>

// Local include(s):
#include "Device.h"

namespace dev {

   // Bring the class(es) into scope:
   using QT_PREPEND_NAMESPACE( QWidget );

   /**
    *  @short Base class for all VME configuration classes
    *
    *         This is much simpler than the base class for the CAMAC
    *         devices, as I didn't get similarly nice ideas as with that.
    *         The VME devices are simply just configured using a simple
    *         "pad".
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class VmeGui : public QWidget,
                  virtual public Device {

      Q_OBJECT

   public:
      /// Standard QWidget style constructor
      VmeGui( QWidget* parent = 0, Qt::WindowFlags flags = 0 )
         : QWidget( parent, flags ) {

         this->resize( WIDGET_WIDTH, WIDGET_HEIGHT );
         this->setMinimumSize( WIDGET_WIDTH, WIDGET_HEIGHT );
         this->setMaximumSize( WIDGET_WIDTH, WIDGET_HEIGHT );
       }

      /// Fixed width of the widget
      static const int WIDGET_WIDTH = 500;
      /// Fixed height of the widget
      static const int WIDGET_HEIGHT = 350;

   }; // class VmeGui

} // namespace dev

#endif // CDA_CORE_DEVICES_VMEGUI_H
