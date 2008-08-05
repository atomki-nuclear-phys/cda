// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_APPS_CDA_CONFIG_EDITOR_CONFIGEDITORWINDOW_H
#define CDA_APPS_CDA_CONFIG_EDITOR_CONFIGEDITORWINDOW_H

// Qt include(s):
#include <QtCore/QString>
#include <QtGui/QMainWindow>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/msg/Logger.h"
#else
#   include "msg/Logger.h"
#endif

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdagui/network/NetworkEditor.h"
#else
#   include "network/NetworkEditor.h"
#endif

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QListWidget )
QT_FORWARD_DECLARE_CLASS( QListWidgetItem )
namespace dev {
   class Editor;
}

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

   /// Slot showing a window about the Qt version used
   void aboutQtSlot();
   /// Slot showing a window about CDA
   void aboutCDASlot();
   /// Slot showing a window about this application
   void aboutConfigEditorSlot();
   void setupSelectActivated( QListWidgetItem * item );
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
/// Configuration read error dialog, and log
   void readErrorDlg();
   /// Widget holding all the shown widgets
   QWidget*         m_centralWidget;
   /// Widget holding the icons
   QListWidget*     m_setupSelect;
   /// Icon for the CAMAC crate settings
   QListWidgetItem* m_deviceSetup;

   /// Icon for the Network settings
   QListWidgetItem* m_networkSetup;

   /// Widget to modify the CAMAC crate settings
   dev::Editor*     m_devEdit;
	net::NetworkEditor *m_netEdit;
   /// Name of the currently "opened" file
   QString          m_currFileName;
   /// Message logging object
   mutable msg::Logger m_logger;

}; // class ConfigEditorWindow

#endif // CDA_APPS_CDA_CONFIG_EDITOR_CONFIGEDITORWINDOW_H
