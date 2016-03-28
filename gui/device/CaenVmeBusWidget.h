// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_GUI_DEVICE_CAENVMEBUSWIDGET_H
#define CDA_GUI_DEVICE_CAENVMEBUSWIDGET_H

// Qt include(s):
#include <QWidget>

namespace dev {

   /**
     * @short Widget for configuring the CAEN VME bus connection parameters
     *
     * All CAEN VME devices in a configuration are assumed to connect to a
     * single VME crate. Which is connected to using the caen::VmeBus class,
     * which uses the CAENVMELib library in the background.
     *
     * This widget is used to configure the settings used to configure the
     * caen::VmeBus object during data taking.
     *
     * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
     *
     * $Revision$
     * $Date$
     */
   class CaenVmeBusWidget : public QWidget {

      Q_OBJECT

   public:
      /// Constructor
      CaenVmeBusWidget( QWidget* parent = 0, Qt::WindowFlags flags = 0 );

      /// Fixed width of the widget
      static const int WIDGET_WIDTH = 510;
      /// Fixed height of the widget
      static const int WIDGET_HEIGHT = 570;

   }; // class CaenVmeBusWidget

} // namespace dev

#endif // CDA_GUI_DEVICE_CAENVMEBUSWIDGET_H
