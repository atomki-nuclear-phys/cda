
// Qt include(s):
#include <QCoreApplication>
#include <QMessageBox>
#include <QApplication>

// Local include(s):
#include "aboutCDA.h"
#include "DefaultFont.h"

/**
 * All the CDA graphical applications should be able to show an information window
 * about CDA itself. In order not to duplicate code too much, the applications should
 * create this window using this function.
 *
 * @param parent Parent of the information window
 */
void aboutCDA( QT_PREPEND_NAMESPACE( QWidget )* parent ) {

   QMessageBox::about( parent, QCoreApplication::translate( "aboutCDA",
                                                            "CDA - Common Data Acquisition" ),
                       QCoreApplication::translate( "aboutCDA", "CDA is an application "
                                                    "suite built for the Experimental Nuclear "
                                                    "Physics department of the Atomki. It is "
                                                    "composed of multiple executables "
                                                    "that work together in the data "
                                                    "acquisition." ) );

   // Make sure Qt remembers what is its default font.
   // (There seems to be a bug in Qt...)
   QApplication::setFont( gui::DefaultFont() );

   return;
}
