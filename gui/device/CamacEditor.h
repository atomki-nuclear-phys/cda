// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_GUI_DEVICE_CAMACEDITOR_H
#define CDA_GUI_DEVICE_CAMACEDITOR_H

// Qt include(s):
#include <QWidget>
#include <QGroupBox>
#include <QStackedWidget>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/IConfig.h"
#   include "cdacore/msg/Logger.h"
#   include "cdacore/common/UniquePtr.h"
#else
#   include "device/IConfig.h"
#   include "msg/Logger.h"
#   include "common/UniquePtr.h"
#endif

// Local include(s):
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
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class CamacEditor : public QWidget,
                       public IConfig {

      Q_OBJECT

   public:
      /// Constructor
      CamacEditor( QWidget* parent = 0, Qt::WindowFlags flags = 0 );

      /// Function reading the configuration in binary format
      virtual bool readConfig( QIODevice& dev );
      /// Function writing the configuration in binary format
      virtual bool writeConfig( QIODevice& dev ) const;
      /// Function reading the configuration in XML format
      virtual bool readConfig( const QDomElement& node );
      /// Function writing the configuration in XML format
      virtual bool writeConfig( QDomElement& node ) const;

      /// Check if this object can read this binary configuration
      bool canRead( QIODevice& dev ) const;
      /// Check if this object can read this XML configuration
      bool canRead( const QDomElement& node ) const;

      /// Clear the configuration
      void clear();

   private slots:
      /// Show the detailed configuration of a CAMAC device
      void showDeviceSlot( int slot );
      /// Remove the detailed configuration of a CAMAC device from view
      void removeDeviceSlot( int slot );

   private:
      /// Widget representing the CAMAC crate
      UniquePtr< CamacCrateWidget >::Type m_crateView;
      /// Area for showing the detailed config
      UniquePtr< QGroupBox >::Type m_deviceBox;
      /// Widget for showing the config
      UniquePtr< QStackedWidget >::Type m_deviceWidget;

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class CamacEditor

} // namespace dev

#endif // CDA_GUI_DEVICE_CAMACEDITOR_H
