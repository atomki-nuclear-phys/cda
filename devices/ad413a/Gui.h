// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_AD413A_GUI_H
#define CDA_DEVICES_AD413A_GUI_H

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
QT_FORWARD_DECLARE_CLASS( QSpinBox )
QT_FORWARD_DECLARE_CLASS( QScrollArea )

namespace ad413a {

   // Forward declaration(s):
   class ChannelGui;

   //
   // Make sure that the following Qt classes are available in the
   // DEV namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QLabel );
   using QT_PREPEND_NAMESPACE( QCheckBox );
   using QT_PREPEND_NAMESPACE( QSpinBox );
   using QT_PREPEND_NAMESPACE( QScrollArea );

   /**
    *  @short Graphical repsesentation of the ORTEC AD413A
    *
    *         This widget is used to change the settings of an ORTEC
    *         AD413A 4 channel Analog-to-Digital Converter. It adheres to the
    *         rules described in the dev::Gui and dev::CrateWidget
    *         classes.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class Gui : public virtual dev::Gui,
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
      /// Slot handling changes to low level discriminants
      void lldChangedSlot( int subaddress, int lld );
      /// Slot handling changes to histogram channel numbers
      void channelsChangedSlot( int subaddress, int channels );
      /// Slot handling changes to histogram lower bounds
      void lowerBoundChangedSlot( int subaddress, double value );
      /// Slot handling changes to histogram upper bounds
      void upperBoundChangedSlot( int subaddress, double value );
      /// Slot handling changes to the LAM setting
      void generateLamChangedSlot( bool on );
      /// Slot handling changes in the gate setting
      void gateChangedSlot( int gate );

   private:
      /// Function "syncing" the configuration
      void sync();

      QScrollArea* m_scrollArea; ///< Object providing the scrolling functions
      QWidget*     m_scrollWidget; ///< Main widget that is scrolled

      QLabel*     m_topLabel; ///< Text label at the top

      QLabel*     m_nameLabel; ///< Label above channel names
      QLabel*     m_lldLabel; ///< Label above low level discriminants
      QLabel*     m_channelsLabel; ///< Label above histogram channels
      QLabel*     m_lowerBoundLabel; ///< Label above histogram lower bounds
      QLabel*     m_upperBoundLabel; ///< Label above histogram upper bounds

      /// The graphical channel representations
      ChannelGui* m_gchannels[ NUMBER_OF_SUBADDRESSES ];

      QCheckBox*  m_generateLamEdit; ///< Widget for changing the LAM setting
      QLabel*     m_gateLabel; ///< Label for the gate spinbox
      QSpinBox*   m_gateEdit; ///< Widget for changing the gate setting

   }; // class Gui

} // namespace ad413a

#endif // CDA_DEVICES_AD413A_GUI_H
