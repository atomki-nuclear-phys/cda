// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_GUI_DEVICE_CRATEWIDGET_H
#define CDA_GUI_DEVICE_CRATEWIDGET_H

// Qt include(s):
#include <QtGui/QWidget>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/Crate.h"
#   include "cdacore/device/Gui.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "device/Crate.h"
#   include "device/Gui.h"
#   include "msg/Logger.h"
#endif

namespace dev {

   // Forward declaration(s):
   class Loader;

   /**
    *  @short Graphical representation of a CAMAC crate
    *
    *         This is THE class of the graphical configuration. It is a
    *         stylised drawing of a CAMAC crate with the inserted modules.
    *         It's a fixed size, 520x250 pixel widget. Maybe an upgrade
    *         will make it scalable, but for now the fixed size was fine...
    *
    *         It behaves as any crate object would, as it inherits from
    *         the dev::Crate template. Hence it can easily read and
    *         write its own configuration.
    *
    *         It provides a graphical view something like this:
    *
    *         @image html CrateWidget.png
    *
    *         It can tell the user when a slot has been clicked, when
    *         it has been double-clicked, etc. For creating and removing
    *         devices it provides context-menus by right clicking on a
    *         crate slot. All in all, it's a pretty powerful widget. :-)
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class CrateWidget : public QWidget,
                       public Crate< dev::Gui > {

      Q_OBJECT

   public:
      /// Constructor
      CrateWidget( QWidget* parent = 0, Qt::WindowFlags flags = 0 );
      /// Destructor
      ~CrateWidget();

      /// Extended configuration reading from binary file
      virtual bool readConfig( QIODevice* dev );
      /// Extended configuration reading from XML file
      virtual bool readConfig( const QDomNode& node );

      /// Set a Loader to be used by the object
      void setLoader( const Loader* loader );
      /// Get the current Loader used by the object
      const Loader* getLoader() const;

      /// Get the CAMAC device in a given slot
      Gui* getDevice( int slot );

      /// Width of a crate slot
      static const int SLOT_WIDTH;
      /// Height of a crate slot
      static const int SLOT_HEIGHT;
      /// Size of the border around the crate
      static const int BORDER_SIZE;

      /// Total width of the widget
      static const int WIDTH;
      /// Total height of the widget
      static const int HEIGHT;

   signals:
      /// Signal for clicking a device
      /**
       * This signal is emitted when one of the crate slots is clicked
       * once with the left (primary) mouse button.
       *
       * @param slot The crate slot that was clicked
       */
      void clicked( int slot );
      /// Signal for double-clicking a device
      /**
       * This signal is emitted when one of the crate slots is
       * double-clicked with the left (primary) mouse button. Note, that
       * this signal is always preceded by the clicked(int) signal.
       *
       *  @param slot The crate slot that was double-clicked
       */
      void doubleClicked( int slot );

   public slots:
      /// Set the device in a given slot
      void setDevice( int slot, Gui* device );

   private slots:
      /// Create a new device in a given slot
      void createSlot( int slot, const QString& type );
      /// Clear a given slot
      void clearSlot( int slot );

   protected:
      /// Function drawing the widget
      virtual void paintEvent( QPaintEvent* event );
      /// Function handling mouse clicks
      virtual void mousePressEvent( QMouseEvent* event );
      /// Function handling double-clicks
      virtual void mouseDoubleClickEvent( QMouseEvent* event );

   private:
      const Loader*       m_loader; ///< Object for creating CAMAC modules
      mutable msg::Logger m_logger; ///< Message logging object

   }; // class CrateWidget

} // namespace dev

#endif // CDA_GUI_DEVICE_CRATEWIDGET_H
