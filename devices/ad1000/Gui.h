// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_AD1000_GUI_H
#define CDA_DEVICES_AD1000_GUI_H

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/Gui.h"
#else
#   include "device/Gui.h"
#endif

// Local include(s):
#include "Device.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QLabel )
QT_FORWARD_DECLARE_CLASS( QCheckBox )

namespace ad1000 {

   // Bring the Qt classes into this namespace:
   using QT_PREPEND_NAMESPACE( QLabel );
   using QT_PREPEND_NAMESPACE( QCheckBox );

   // Forward declaration(s):
   class ChannelGui;

   /**
    *  @short Graphical repsesentation of the AD1000 ADC
    *
    *         This widget is used to change the settings of an Atomki
    *         AD1000 ADC.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class Gui : public dev::Gui,
               public virtual Device {

      Q_OBJECT

   public:
      /// Constructor
      Gui( QWidget* parent = 0, Qt::WindowFlags flags = 0 );
      /// Destructor
      ~Gui();

      /// Read the device configuration from a binary file
      virtual bool readConfig( QIODevice* dev );
      /// Read the device configuration from an XML file
      virtual bool readConfig( const QDomElement& node );

      /// Draw a graphical representation of the module
      virtual void drawModule( QPainter& painter ) const;
      /// Width of the device in units of crate slots
      virtual int deviceWidth() const;

   private slots:
      /// Slot handling changes to channel names
      void nameChangedSlot( const QString& text );
      /// Slot handling changes to histogram channel numbers
      void channelsChangedSlot( int channels );
      /// Slot handling changes to histogram lower bounds
      void lowerBoundChangedSlot( double value );
      /// Slot handling changes to histogram upper bounds
      void upperBoundChangedSlot( double value );
      /// Slot handling changes to the LAM setting
      void generateLamChangedSlot( bool on );

   private:
      /// Function "syncing" the configuration
      void sync();

      QLabel*     m_topLabel; ///< Text label at the top

      QLabel*     m_nameLabel; ///< Label above channel names
      QLabel*     m_channelsLabel; ///< Label above histogram channels
      QLabel*     m_lowerBoundLabel; ///< Label above histogram lower bounds
      QLabel*     m_upperBoundLabel; ///< Label above histogram upper bounds
      ChannelGui* m_gchannel; ///< The graphical channel representation

      QCheckBox*  m_generateLamEdit; ///< Widget for changing the LAM setting

   }; // class Gui

} // namespace ad1000

#endif // CDA_DEVICES_AD1000_GUI_H
