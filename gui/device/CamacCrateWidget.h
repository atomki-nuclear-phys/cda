// Dear emacs, this is -*- c++ -*-
#ifndef CDA_GUI_DEVICE_CAMACCRATEWIDGET_H
#define CDA_GUI_DEVICE_CAMACCRATEWIDGET_H

// System include(s):
#include <memory>

// Qt include(s):
#include <QWidget>

// CDA include(s):
#include "device/Crate.h"
#include "device/CamacGui.h"
#include "msg/Logger.h"

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
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    */
   class CamacCrateWidget : public QWidget,
                            public Crate< dev::CamacGui > {

      Q_OBJECT

   public:
      /// Constructor
      CamacCrateWidget( QWidget* parent = 0, Qt::WindowFlags flags = 0 );

      /// Extended configuration reading from binary file
      virtual StatusCode readConfig( QIODevice& dev );
      /// Extended configuration reading from XML file
      virtual StatusCode readConfig( const QDomElement& element );

      /// Get the CAMAC device in a given slot
      CamacGui* getDevice( int slot );
      /// Set the device in a given slot
      void setDevice( int slot, std::unique_ptr< CamacGui > device );

      /// Maximal number of slots the devices can use
      static const int NUMBER_OF_SLOTS;

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
      mutable msg::Logger m_logger; ///< Message logging object

   }; // class CamacCrateWidget

} // namespace dev

#endif // CDA_GUI_DEVICE_CAMACCRATEWIDGET_H
