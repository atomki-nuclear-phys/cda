// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_DEVICE_CAMACGUI_H
#define CDA_CORE_DEVICE_CAMACGUI_H

// Qt include(s):
#include <QtGui/QWidget>

// Local include(s):
#include "Device.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QPainter )

namespace dev {

   //
   // Make sure that the following Qt classes are available in the
   // DEV namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QWidget );
   using QT_PREPEND_NAMESPACE( QPainter );

   /**
    *  @short Base class for the graphical representation of a device
    *
    *         All devices have to be able to show their configuration in
    *         a graphical way. This interface is also used to change
    *         the configuration of the devices.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class CamacGui : public QWidget,
                    virtual public Device {

      Q_OBJECT

   public:
      /// Standard QWidget style constructor
      CamacGui( QWidget* parent = 0, Qt::WindowFlags flags = 0 )
         : QWidget( parent, flags ) {

         this->resize( WIDGET_WIDTH, WIDGET_HEIGHT );
         this->setMinimumSize( WIDGET_WIDTH, WIDGET_HEIGHT );
         this->setMaximumSize( WIDGET_WIDTH, WIDGET_HEIGHT );
      }

      /// Draw an illustration of the module
      /**
       * Each CAMAC module can (has to) draw a graphical representation
       * of itself. This has to be a 20x200 pixel drawing. The QPainter
       * object received by the module is set up such that its 0,0
       * coordinate is the upper-left corner of the module area.
       *
       * @param painter The object to draw the module with
       */
      virtual void drawModule( QPainter& painter ) const = 0;

      /// Width of the device in units of crate slots
      /**
       * Most of the CAMAC devices are just 1 slot wide. But some of them
       * are actually 2 slots wide. The configuration application has to
       * be able to handle this.
       */
      virtual int deviceWidth() const = 0;

      /// Fixed width of a device
      static const int WIDGET_WIDTH = 500;
      /// Fixed height of a device
      static const int WIDGET_HEIGHT = 350;

   signals:
      /// Request the redrawing of the module illustration
      /**
       * The graphical representation of the module can be configuration
       * dependent. (Show which channels are read out, etc.) If the
       * configuration of the module is changed and now it would have
       * to be drawn differently, it has to emit this signal. It will
       * result in the eventual calling of the drawModule() function.
       */
      void redrawModule();

   }; // class CamacGui

} // namespace dev

#endif // CDA_CORE_DEVICE_CAMACGUI_H
