// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_T4300B_GUI_H
#define CDA_DEVICES_T4300B_GUI_H

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
QT_FORWARD_DECLARE_CLASS( QScrollArea )

namespace t4300b {

   // Forward declaration(s):
   class ChannelGui;

   //
   // Make sure that the following Qt classes are available in the
   // DEV namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QLabel );
   using QT_PREPEND_NAMESPACE( QCheckBox );
   using QT_PREPEND_NAMESPACE( QScrollArea );

   /**
    *  @short Graphical repsesentation of the T4300B QDC
    *
    *         This widget is used to change the settings of a
    *         T4300B 16 channel Charge-to-Digital Converter. It adheres to the
    *         rules described in the dev::Gui and dev::CrateWidget
    *         classes.
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
      /// Slot handling changes to channel enablement
      void channelEnabledSlot( int subaddress, bool on );
      /// Slot handling changes to channel names
      void nameChangedSlot( int subaddress, const QString& text );
      /// Slot handling changes to histogram channel numbers
      void channelsChangedSlot( int subaddress, int channels );
      /// Slot handling changes to histogram lower bounds
      void lowerBoundChangedSlot( int subaddress, double value );
      /// Slot handling changes to histogram upper bounds
      void upperBoundChangedSlot( int subaddress, double value );
      /// Slot handling changes to the LAM setting
      void generateLamChangedSlot( bool on );

   private:
      /// Function "syncing" the configuration
      void sync();

      QScrollArea* m_scrollArea; ///< Object providing the scrolling functions
      QWidget*     m_scrollWidget; ///< Main widget that is scrolled

      QLabel*     m_topLabel; ///< Text label at the top

      QLabel*     m_nameLabel; ///< Label above channel names
      QLabel*     m_channelsLabel; ///< Label above histogram channels
      QLabel*     m_lowerBoundLabel; ///< Label above histogram lower bounds
      QLabel*     m_upperBoundLabel; ///< Label above histogram upper bounds

      /// The graphical channel representations
      ChannelGui* m_gchannels[ NUMBER_OF_SUBADDRESSES ];

      QCheckBox*  m_generateLamEdit; ///< Widget for changing the LAM setting

   }; // class Gui

} // namespace t4300b

#endif // CDA_DEVICES_T4300B_GUI_H
