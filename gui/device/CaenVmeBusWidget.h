// Dear emacs, this is -*- c++ -*-
#ifndef CDA_GUI_DEVICE_CAENVMEBUSWIDGET_H
#define CDA_GUI_DEVICE_CAENVMEBUSWIDGET_H

// System include(s):
#include <memory>

// Qt include(s):
#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>

// CDA include(s):
#include "caen/VmeBus.h"

// Local include(s):
#include "../common/Export.h"

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
     */
   class CDAGUI_EXPORT CaenVmeBusWidget : public QWidget {

      Q_OBJECT

   public:
      /// Constructor
      CaenVmeBusWidget( QWidget* parent = 0, Qt::WindowFlags flags = 0 );

      /// Fixed width of the widget
      static const int WIDGET_WIDTH = 510;
      /// Fixed height of the widget
      static const int WIDGET_HEIGHT = 570;

      /// Get the controller type
      caen::VmeBus::BoardType type() const;
      /// Set the controller type
      void setType( caen::VmeBus::BoardType value );

      /// Get the link number
      short linkNumber() const;
      /// Set the link number
      void setLinkNumber( short value );

      /// Get the board number
      short boardNumber() const;
      /// Set the board number
      void setBoardNumber( short value );

      /// Get whether the synchronisation between the devices should be checked
      bool checkDeviceSync() const;
      /// Set whether the synchronisation between the devices should be checked
      void setCheckDeviceSync( bool value );

      // Reset the configuration to its default values
      void reset();

   signals:
      /// Signal emitted when the controller type is modified
      void typeModified( caen::VmeBus::BoardType type );
      /// Signal emitted when the controller link number is modified
      void linkNumberModified( short value );
      /// Signal emitted when the controller board number is modified
      void boardNumberModified( short value );
      /// Signal emitted when the device synchronisation check is modified
      void checkDeviceSyncModified( bool value );

   private slots:
      /// Slot catching type change signals
      void typeModifiedSlot( int index );
      /// Slot catching link number change signals
      void linkNumberModifiedSlot( int value );
      /// Slot catching board number change signals
      void boardNumberModifiedSlot( int value );
      /// Slot catching device synchronisation change signals
      void checkDeviceSyncModifiedSlot( bool value );

   private:
      /// Image of a CAEN VME crate
      std::unique_ptr< QLabel > m_image;
      /// Label describing the widget
      std::unique_ptr< QLabel > m_label;

      /// Label describing the type selector
      std::unique_ptr< QLabel > m_typeLabel;
      /// Dropdown menu selecting the controller type
      std::unique_ptr< QComboBox > m_type;

      /// Label describing the link number selector
      std::unique_ptr< QLabel > m_linkNumberLabel;
      /// Spinbox selecting the link number
      std::unique_ptr< QSpinBox > m_linkNumber;

      /// Label describing the board number selector
      std::unique_ptr< QLabel > m_boardNumberLabel;
      /// Spinbox selecting the board number
      std::unique_ptr< QSpinBox > m_boardNumber;

      /// Check box for controlling the device synchronisation checks
      std::unique_ptr< QCheckBox > m_checkDeviceSync;

   }; // class CaenVmeBusWidget

} // namespace dev

#endif // CDA_GUI_DEVICE_CAENVMEBUSWIDGET_H
