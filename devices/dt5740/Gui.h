// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_DT5740_GUI_H
#define CDA_DEVICES_DT5740_GUI_H

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/CaenGui.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "device/CaenGui.h"
#   include "msg/Logger.h"
#endif

// Local include(s):
#include "Device.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QLabel )
QT_FORWARD_DECLARE_CLASS( QCheckBox )
QT_FORWARD_DECLARE_CLASS( QScrollArea )
QT_FORWARD_DECLARE_CLASS( QGroupBox )
QT_FORWARD_DECLARE_CLASS( QComboBox )
QT_FORWARD_DECLARE_CLASS( QSpinBox )

namespace dt5740 {

   // Forward declaration(s):
   class GroupGui;

   //
   // Make sure that the following Qt classes are available in the
   // DEV namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QLabel );
   using QT_PREPEND_NAMESPACE( QCheckBox );
   using QT_PREPEND_NAMESPACE( QScrollArea );

   /**
    *  @short Graphical repsesentation of the DT5640 digitizer
    *
    *         This widget is used to change the settings of a
    *         CAEN DT5740 digitizer.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class Gui : public dev::CaenGui,
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

   private slots:
      /// Slot handling changes to the connection mode
      void connectionModeSlot( int index );
      /// Slot handling changes to the connection link
      void connectionLinkSlot( int link );

      /// Slot handling changes to channel enablement
      void channelEnabledSlot( int channel, bool on );
      /// Slot handling changes to channel names
      void nameChangedSlot( int channel, const QString& text );
      /// Slot handling changes to histogram channel numbers
      void channelsChangedSlot( int channel, int channels );
      /// Slot handling changes to histogram lower bounds
      void lowerBoundChangedSlot( int channel, double value );
      /// Slot handling changes to histogram upper bounds
      void upperBoundChangedSlot( int channel, double value );

      /// Slot handling changes to self trigger enablement
      void trigEnabledSlot( int group, bool state );
      /// Slot handling changes to the trigger output enablement
      void trigOutEnabledSlot( int group, bool state );
      /// Slot handling changes to the trigger overlap state
      void trigOvlpEnabledSlot( int group, bool state );
      /// Slot handling changes to the trigger mode
      void trigModeSlot( int group, dt5740::GroupConfig::TriggerMode mode );
      /// Slot handling changes to the trigger threshold
      void trigThresholdSlot( int group, int value );
      /// Slot handling changes to the trigger masks
      void trigMaskSlot( int group, unsigned int value );
      /// Slot handling changes to the post trigger samples value
      void postTrigSamplesSlot( int group, int value );
      /// Slot handling changes to the DC offset values
      void dcOffsetSlot( int group, int value );
      /// Slot handling changes to the pattern generation state
      void patGenEnabledSlot( int group, bool state );
      /// Slot handling changes to the gate modes
      void gateModeSlot( int group, dt5740::GroupConfig::GateMode mode );
      /// Slot handling changes to the buffer modes
      void bufferModeSlot( int group, dt5740::GroupConfig::BufferMode mode );

   private:
      /// Function "syncing" the configuration
      void sync();

      QScrollArea* m_scrollArea; ///< Object providing the scrolling functions
      QWidget*     m_scrollWidget; ///< Main widget that is scrolled

      QLabel*     m_image; ///< Image of the digitizer itself
      QLabel*     m_topLabel; ///< Text label at the top

      QGroupBox* m_connectionBox; ///< Box for the connection parameters
      QLabel*    m_connModeLabel; ///< Description for conn. mode setting
      QComboBox* m_connMode;      ///< Connection mode chooser
      QLabel*    m_connLinkLabel; ///< Description for conn. link setting
      QSpinBox*  m_connLink;      ///< Connection link chooser

      /// The graphical representation of channel groups
      GroupGui* m_ggroups[ NUMBER_OF_GROUPS ];

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class Gui

} // namespace dt5740

#endif // CDA_DEVICES_DT5740_GUI_H
