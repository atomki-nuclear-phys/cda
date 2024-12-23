// Dear emacs, this is -*- c++ -*-
#ifndef CDA_GUI_DEVICE_CAENDIGITIZEREDITOR_H
#define CDA_GUI_DEVICE_CAENDIGITIZEREDITOR_H

// System include(s):
#include <memory>

// Qt include(s):
#include <QComboBox>
#include <QTabWidget>
#include <QWidget>

// CDA include(s):
#include "device/CaenDigitizerGui.h"
#include "device/Crate.h"
#include "msg/Logger.h"

// Local include(s):
#include "../common/Export.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS(QTabWidget)
QT_FORWARD_DECLARE_CLASS(QComboBox)

namespace dev {

/**
 *  @short Editor widget for CAEN digitizer devices
 *
 *         This class is used to construct/view the configuration
 *         of (a) CAEN digitizer device(s) for a CDA data taking session.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */
class CDAGUI_EXPORT CaenDigitizerEditor
    : public QWidget,
      public dev::Crate<dev::CaenDigitizerGui> {

   Q_OBJECT

public:
   /// Standard QWidget style constructor
   CaenDigitizerEditor(QWidget* parent = 0,
                       Qt::WindowFlags flags = Qt::WindowFlags());
   /// Destructor
   ~CaenDigitizerEditor();

   /// Function reading the configuration in binary format
   virtual StatusCode readConfig(QIODevice& dev);
   /// Function reading the configuration in XML format
   virtual StatusCode readConfig(const QDomElement& node);

   /// Re-implemented clear function, resetting the widget
   virtual void clear();

private slots:
   /// Create a device in the configuration
   void createDeviceSlot(int index);
   /// Remove the current device from the configuration
   void deleteDeviceSlot(int index);
   /// Handle the change in the connection configuration of a device
   void idChangedSlot();

private:
   /// Function checking if the GUI is consistent with the configuration
   bool consistent() const;

   /// A widget to show the device(s) in
   std::unique_ptr<QTabWidget> m_deviceTab;
   /// Dropdown menu selecting what kind of device to create
   std::unique_ptr<QComboBox> m_createDevice;

   mutable msg::Logger m_logger;  ///< Message logger object

};  // class CaenDigitizerEditor

}  // namespace dev

#endif  // CDA_GUI_DEVICE_CAENDIGITIZEREDITOR_H
