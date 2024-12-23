// Dear emacs, this is -*- c++ -*-
#ifndef CDA_GUI_DEVICE_CAMACEDITOR_H
#define CDA_GUI_DEVICE_CAMACEDITOR_H

// System include(s):
#include <memory>

// Qt include(s):
#include <QGroupBox>
#include <QStackedWidget>
#include <QWidget>

// CDA include(s):
#include "device/IConfig.h"
#include "msg/Logger.h"

// Local include(s):
#include "../common/Export.h"
#include "CamacCrateWidget.h"

namespace dev {

/**
 *  @short Class used for editing the CAMAC crate setup
 *
 *         This class can be used to graphically edit the settings
 *         of a CAMAC crate. It displays the crate using a CrateWidget
 *         widget, and it shows the detailed configuration of the
 *         selected device below the crate.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */
class CDAGUI_EXPORT CamacEditor : public QWidget, public IConfig {

   Q_OBJECT

public:
   /// Constructor
   CamacEditor(QWidget* parent = 0, Qt::WindowFlags flags = Qt::WindowFlags());
   /// Destructor
   ~CamacEditor();

   /// Function reading the configuration in binary format
   virtual StatusCode readConfig(QIODevice& dev);
   /// Function writing the configuration in binary format
   virtual StatusCode writeConfig(QIODevice& dev) const;
   /// Function reading the configuration in XML format
   virtual StatusCode readConfig(const QDomElement& node);
   /// Function writing the configuration in XML format
   virtual StatusCode writeConfig(QDomElement& node) const;

   /// Check if this object can read this binary configuration
   bool canRead(QIODevice& dev) const;
   /// Check if this object can read this XML configuration
   bool canRead(const QDomElement& node) const;

   /// Clear the configuration
   void clear();

private slots:
   /// Show the detailed configuration of a CAMAC device
   void showDeviceSlot(int slot);
   /// Remove the detailed configuration of a CAMAC device from view
   void removeDeviceSlot(int slot);

private:
   /// Widget representing the CAMAC crate
   std::unique_ptr<CamacCrateWidget> m_crateView;
   /// Area for showing the detailed config
   std::unique_ptr<QGroupBox> m_deviceBox;
   /// Widget for showing the config
   std::unique_ptr<QStackedWidget> m_deviceWidget;

   mutable msg::Logger m_logger;  ///< Message logger object

};  // class CamacEditor

}  // namespace dev

#endif  // CDA_GUI_DEVICE_CAMACEDITOR_H
