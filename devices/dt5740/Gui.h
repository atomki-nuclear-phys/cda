// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_DT5740_GUI_H
#define CDA_DEVICES_DT5740_GUI_H

// Qt include(s):
#include <QtCore/QtGlobal>
#include <QLabel>
#include <QCheckBox>
#include <QScrollArea>
#include <QGroupBox>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/CaenGui.h"
#   include "cdacore/msg/Logger.h"
#   include "cdacore/common/UniquePtr.h"
#else
#   include "device/CaenGui.h"
#   include "msg/Logger.h"
#   include "common/UniquePtr.h"
#endif

// Local include(s):
#include "Device.h"
#include "GroupGui.h"

namespace dt5740 {

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

      /// Read the device configuration from a binary file
      virtual bool readConfig( QIODevice& dev );
      /// Read the device configuration from an XML file
      virtual bool readConfig( const QDomElement& node );

   private slots:
      /// Slot handling changes to the connection mode
      void connectionModeSlot( int index );
      /// Slot handling changes to the connection link
      void connectionLinkSlot( int link );

      /// Slot handling trigger overlap state changes
      void trigOvlpEnabledSlot( bool checked );
      /// Slot handling external trigger changes
      void extTrigEnabledSlot( bool checked );
      /// Slot handling external forwarding trigger changes
      void extTrigOutEnabledSlot( bool checked );
      /// Slot handling trigger mode changes
      void trigModeSlot( int index );
      /// Slot handling post trigger percentage changes
      void postTrigPercentageSlot( int value );
      /// Slot handling GPO impedance changes
      void highImpedanceGPOSlot( bool checked );
      /// Slot handling front panel signal type changes
      void signalTypeSlot( int index );

      /// Slot handling changes to the pattern generation state
      void patGenEnabledSlot( bool checked );
      /// Slot handling changes to the raw ntuple saving state
      void saveRawNtupleSlot( bool checked );
      /// Slot handling the acquisition mode changes
      void acqModeSlot( int index );
      /// Slot handling the gate mode changes
      void gateModeSlot( int index );
      /// Slot handling the buffer mode changes
      void bufferModeSlot( int index );
      /// Slot handling the clock source changes
      void clockSourceSlot( int index );
      /// Slot handling the event counting mode changes
      void evCountModeSlot( int index );

      /// Slot handling the CFD fraction changes
      void cfdFractionSlot( double value );
      /// Slot handling the CFD delay changes
      void cfdDelaySlot( int value );
      /// Slot handling the CFD length changes
      void cfdLengthSlot( int value );

      /// Slot handling the Gauss. smooth width changes
      void gaussSmoothWidthSlot( double value );

   private:
      /// Function "syncing" the configuration
      void sync();

      /// Object providing the scrolling functions
      UniquePtr< QScrollArea >::Type m_scrollArea;
      /// Main widget that is scrolled
      UniquePtr< QWidget >::Type m_scrollWidget;

      /// Image of the digitizer itself
      UniquePtr< QLabel >::Type m_image;
      /// Text label at the top
      UniquePtr< QLabel >::Type m_topLabel;

      /// Box for the connection parameters
      UniquePtr< QGroupBox >::Type m_connectionBox;
      /// Description for conn. mode setting
      UniquePtr< QLabel >::Type m_connModeLabel;
      /// Connection mode chooser
      UniquePtr< QComboBox >::Type m_connMode;
      /// Description for conn. link setting
      UniquePtr< QLabel >::Type m_connLinkLabel;
      /// Connection link chooser
      UniquePtr< QSpinBox >::Type m_connLink;

      /// Box for the trigger settings
      UniquePtr< QGroupBox >::Type m_triggerBox;
      /// Setting for enabling trigger overlaps
      UniquePtr< QCheckBox >::Type m_trigOvlpEnabledWidget;
      /// External trigger setting
      UniquePtr< QCheckBox >::Type m_extTrigEnabledWidget;
      /// External trigger forward setting
      UniquePtr< QCheckBox >::Type m_extTrigOutEnabledWidget;
      /// GPO impedance level setting
      UniquePtr< QCheckBox >::Type m_highImpedanceGPOWidget;
      /// Description for post trigger fraction
      UniquePtr< QLabel >::Type m_postTrigPercentageLabel;
      /// Post trigger fraction setting
      UniquePtr< QSpinBox >::Type m_postTrigPercentageWidget;
      /// Description of the trigger mode setting
      UniquePtr< QLabel >::Type m_trigModeLabel;
      /// Trigger mode chooser
      UniquePtr< QComboBox >::Type m_trigModeWidget;
      /// Description for front panel signal type
      UniquePtr< QLabel >::Type m_signalTypeLabel;
      /// Front panel signal type setting
      UniquePtr< QComboBox >::Type m_signalTypeWidget;

      /// Box for the acquisition settings
      UniquePtr< QGroupBox >::Type m_acquisitionBox;
      /// Pattern generation setting
      UniquePtr< QCheckBox >::Type m_patGenEnabledWidget;
      /// RAW ntuple save setting
      UniquePtr< QCheckBox >::Type m_saveRawNtupleWidget;
      /// Description of the acquisition mode setting
      UniquePtr< QLabel >::Type m_acqModeLabel;
      /// Acquisition mode chooser
      UniquePtr< QComboBox >::Type m_acqModeWidget;
      /// Description of the gate mode setting
      UniquePtr< QLabel >::Type m_gateModeLabel;
      /// Gate mode chooser
      UniquePtr< QComboBox >::Type m_gateModeWidget;
      /// Description of buffer mode setting
      UniquePtr< QLabel >::Type m_bufferModeLabel;
      /// Buffer mode chooser
      UniquePtr< QComboBox >::Type m_bufferModeWidget;
      /// Description of the clock source setting
      UniquePtr< QLabel >::Type m_clockSourceLabel;
      /// Clock source chooser
      UniquePtr< QComboBox >::Type m_clockSourceWidget;
      /// Description of the event counting mode setting
      UniquePtr< QLabel >::Type m_evCountModeLabel;
      /// Event counting mode chooser
      UniquePtr< QComboBox >::Type m_evCountModeWidget;

      /// Box for the signal reconstruction settings
      UniquePtr< QGroupBox >::Type m_recoBox;
      /// Description of the CFD fraction setting
      UniquePtr< QLabel >::Type m_cfdFractionLabel;
      /// Digital CFD fraction setting
      UniquePtr< QDoubleSpinBox >::Type m_cfdFractionWidget;
      /// Description of the CFD delay setting
      UniquePtr< QLabel >::Type m_cfdDelayLabel;
      /// Digital CFD delay setting
      UniquePtr< QSpinBox >::Type m_cfdDelayWidget;
      /// Description of the CFD length setting
      UniquePtr< QLabel >::Type m_cfdLengthLabel;
      /// Digital CFD length setting
      UniquePtr< QSpinBox >::Type m_cfdLengthWidget;
      /// Description of the Gaussian smoothing width setting
      UniquePtr< QLabel >::Type m_gaussSmoothWidthLabel;
      /// Gaussian smoothing width setting
      UniquePtr< QDoubleSpinBox >::Type m_gaussSmoothWidthWidget;

      /// The graphical representation of channel groups
      UniquePtr< GroupGui >::Type m_ggroups[ NUMBER_OF_GROUPS ];

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class Gui

} // namespace dt5740

#endif // CDA_DEVICES_DT5740_GUI_H
