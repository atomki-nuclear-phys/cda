// Dear emacs, this is -*- c++ -*-
#ifndef CDA_GUI_DEVICE_CAENVMEEDITOR_H
#define CDA_GUI_DEVICE_CAENVMEEDITOR_H

// System include(s):
#include <memory>

// Qt include(s):
#include <QWidget>
#include <QTabWidget>
#include <QComboBox>

// CDA include(s):
#include "caen/VmeBus.h"
#include "device/Crate.h"
#include "device/CaenVmeGui.h"
#include "msg/Logger.h"

// Local include(s):
#include "../common/Export.h"
#include "CaenVmeBusWidget.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QTabWidget )
QT_FORWARD_DECLARE_CLASS( QComboBox )

namespace dev {

   /**
    *  @short Editor widget for CAEN VME devices
    *
    *         This class is used to construct/view the configuration
    *         of (a) CAEN VME device(s) for a CDA data taking session.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    */
   class CDAGUI_EXPORT CaenVmeEditor :
      public QWidget,
      public dev::Crate< dev::CaenVmeGui > {

      Q_OBJECT

   public:
      /// Standard QWidget style constructor
      CaenVmeEditor( QWidget* parent = 0, Qt::WindowFlags flags = 0 );
      /// Destructor
      ~CaenVmeEditor();

      /// Function reading the configuration in binary format
      virtual StatusCode readConfig( QIODevice& dev );
      /// Function reading the configuration in XML format
      virtual StatusCode readConfig( const QDomElement& node );

      /// Re-implemented clear function, resetting the widget
      virtual void clear();

   protected:
      /// Read the crate specific options from binary input
      virtual StatusCode readCrateConfig( QIODevice& dev );
      /// Write the crate specific options to binary output
      virtual StatusCode writeCrateConfig( QIODevice& dev ) const;

      /// Read the crate specific options from XML input
      virtual StatusCode readCrateConfig( const QDomElement& node );
      /// Write the crate specific options to XML output
      virtual StatusCode writeCrateConfig( QDomElement& node ) const;

   private slots:
      /// Create a device in the configuration
      void createDeviceSlot( int index );
      /// Remove the current device from the configuration
      void deleteDeviceSlot( int index );
      /// Handle the change in the connection configuration of a device
      void addressChangedSlot();

   private:
      /// Function checking if the GUI is consistent with the configuration
      bool consistent() const;

      /// A widget to show the device(s) in
      std::unique_ptr< QTabWidget > m_deviceTab;
      /// Dropdown menu selecting what kind of device to create
      std::unique_ptr< QComboBox > m_createDevice;

      /// Widget for editing the VME crate connection parameters
      std::unique_ptr< CaenVmeBusWidget > m_vmeWidget;

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class CaenVmeEditor

} // namespace dev

#endif // CDA_GUI_DEVICE_CAENVMEEDITOR_H
