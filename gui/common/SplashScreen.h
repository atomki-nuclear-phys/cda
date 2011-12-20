// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_GUI_COMMON_SPLASHSCREEN_H
#define CDA_GUI_COMMON_SPLASHSCREEN_H

// Qt include(s):
#include <QtGui/QSplashScreen>

namespace gui {

   /**
    *  @short Simple extension to QSplashScreen
    *
    *         QSplashScreen can not be easily displayed on the screen once
    *         the main window of the application is shown. This small extension
    *         on top of the Qt class makes it possible to specify for how many
    *         seconds the splash screen should be shown even after the
    *         application's main window is ready.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class SplashScreen : public QSplashScreen {

      Q_OBJECT

   public:
      /// Constructor specifying a timeout
      SplashScreen( const QPixmap& pixmap, Qt::WindowFlags f = 0,
                    int timeout = 3 );

   private slots:
      /// Slot hiding the splash screen at the end of the timeout
      void hideSplash();

   }; // SplashScreen

} // namespace gui

#endif // CDA_GUI_COMMON_SPLASHSCREEN_H
