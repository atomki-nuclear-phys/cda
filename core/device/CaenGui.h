// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_DEVICE_CAENGUI_H
#define CDA_CORE_DEVICE_CAENGUI_H

// Qt include(s):
#include <QWidget>

// Local include(s):
#include "Device.h"

namespace dev {

   //
   // Make sure that the following Qt classes are available in the
   // DEV namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QWidget );
   using QT_PREPEND_NAMESPACE( QPainter );

   /**
    *  @short Base class for the graphical representation of a CAEN device
    *
    *         All CAEN devices have to be able to show their configuration in
    *         a graphical way. This interface is also used to change
    *         the configuration of the devices.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class CaenGui : public QWidget,
                   virtual public Device {

      Q_OBJECT

   public:
      /// Standard QWidget style constructor
      CaenGui( QWidget* parent = 0, Qt::WindowFlags flags = 0 );

      /// Fixed width of a device
      static const int WIDGET_WIDTH = 510;
      /// Fixed height of a device
      static const int WIDGET_HEIGHT = 570;

   signals:
      /// Signal emitted when the ID of the device is modified
      /**
       * The ID of CAEN devices is calculated from their connection
       * parameters. Since these can be modified after the device has
       * been created, the crate holding them has to listen to such
       * signals, and re-order them in its internal map when one of them
       * modifies its connection parameters.
       */
      void idChanged();

   }; // class CaenGui

} // namespace dev

#endif // CDA_CORE_DEVICE_CAENGUI_H
