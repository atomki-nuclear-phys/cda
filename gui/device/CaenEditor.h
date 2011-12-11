// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_GUI_DEVICE_CAENEDITOR_H
#define CDA_GUI_DEVICE_CAENEDITOR_H

// Qt include(s):
#include <QtGui/QWidget>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/Crate.h"
#   include "cdacore/device/CaenGui.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "device/Crate.h"
#   include "device/CaenGui.h"
#   include "msg/Logger.h"
#endif

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QLabel )
QT_FORWARD_DECLARE_CLASS( QStackedWidget )
QT_FORWARD_DECLARE_CLASS( QComboBox )
QT_FORWARD_DECLARE_CLASS( QPushButton )

namespace dev {

   /**
    *  @short Editor widget for CAEN devices
    *
    *         This class is used to construct/view the configuration
    *         of (a) CAEN device(s) for a CDA data taking session.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class CaenEditor : public QWidget,
                      public dev::Crate< dev::CaenGui > {

      Q_OBJECT

   public:
      /// Standard QWidget style constructor
      CaenEditor( QWidget* parent = 0, Qt::WindowFlags flags = 0 );

      /// Function reading the configuration in binary format
      virtual bool readConfig( QIODevice* dev );
      /// Function reading the configuration in XML format
      virtual bool readConfig( const QDomElement& node );

   private slots:
      /// Create a device in the configuration
      void createDeviceSlot( int index );
      /// Remove the current device from the configuration
      void clearDeviceSlot();

   private:
      QStackedWidget* m_deviceStack; ///< A widget to show the device(s) in

      QComboBox* m_createDevice; ///< Dropdown menu selecting what kind of device to create
      bool m_selfModification; ///< Flag showing that the object is modifying itself at the moment
      QPushButton* m_clearDevice; ///< Button for clearing the configuration

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class CaenEditor

} // namespace dev

#endif // CDA_GUI_DEVICE_CAENEDITOR_H
