// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_APPS_CDA_CONFIG_EDITOR_CONFIGEDITORWINDOW_H
#define CDA_APPS_CDA_CONFIG_EDITOR_CONFIGEDITORWINDOW_H

// System include(s):
#include <memory>

// Qt include(s):
#include <QtCore/QString>
#include <QMainWindow>
#include <QStackedWidget>
#include <QAction>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/msg/Logger.h"
#   include "cdagui/device/CamacEditor.h"
#   include "cdagui/device/CaenDigitizerEditor.h"
#else
#   include "msg/Logger.h"
#   include "device/CamacEditor.h"
#   include "device/CaenDigitizerEditor.h"
#endif

/**
 *  @short Main window for the configuration editor application
 *
 *         This is a "classic" main window for the configuration
 *         editor application. It has an icon view on the left that
 *         lets the user switch between the different configuration
 *         aspects, while the actual configuration options are always
 *         shown on the right.
 *
 *         Right now there is only one configuration "aspect", the
 *         configuration of the CAMAC crate. The window should look
 *         something like this:
 *
 *         @image html ConfigEditorWindow.png
 *
 * @author Attila Krasznahorkay Jr.
 *
 * $Revision$
 * $Date$
 */
class ConfigEditorWindow : public QMainWindow {

   Q_OBJECT

public:
   /// Constructor
   ConfigEditorWindow();

private slots:
   /// Slot handling configuration clearing
   void newConfigSlot();
   /// Slot handling the reading of a configuration
   void readConfigSlot();
   /// Slot handling the writing of a configuration
   void writeConfigSlot();
   /// Slot handling the writing of a configuration
   void writeConfigAsSlot();

   /// Slot for showing the CAMAC configuration
   void showCamacConfigSlot();
   /// Slot for showing the CAEN configuration
   void showCaenConfigSlot();

   /// Slot showing a window about CDA
   void aboutCDASlot();
   /// Slot showing a window about this application
   void aboutConfigEditorSlot();

private:
   /// Function creating the application menus
   void createMenus();
   /// Function reading an XML configuration file
   void readXMLConfig( const QString& filename );
   /// Function reading a binary configuration file
   void readBinaryConfig( const QString& filename );
   /// Function writing an XML configuration file
   void writeXMLConfig( const QString& filename );
   /// Function writing a binary configuration file
   void writeBinaryConfig( const QString& filename );

   /// Central widget
   std::unique_ptr< QWidget > m_centralWidget;
   /// Stack of the editor widgets
   std::unique_ptr< QStackedWidget > m_editStack;

   /// Widget to modify the CAMAC crate settings
   std::unique_ptr< dev::CamacEditor > m_camacEdit;
   /// Widget to modify the CAEN device settings
   std::unique_ptr< dev::CaenDigitizerEditor > m_caenDigitizerEdit;

   /// Action triggering the display of the CAMAC confiugration
   std::unique_ptr< QAction > m_camacConfigAction;
   /// Action triggering the display of the CAEN digitizer configuration
   std::unique_ptr< QAction > m_caenDigitizerConfigAction;

   /// Name of the currently "opened" file
   QString m_currFileName;
   /// Message logging object
   mutable msg::Logger m_logger;

}; // class ConfigEditorWindow

#endif // CDA_APPS_CDA_CONFIG_EDITOR_CONFIGEDITORWINDOW_H
