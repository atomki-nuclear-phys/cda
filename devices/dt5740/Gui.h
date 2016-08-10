// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_DT5740_GUI_H
#define CDA_DEVICES_DT5740_GUI_H

// System include(s):
#include <memory>

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
#include "device/CaenDigitizerGui.h"
#include "msg/Logger.h"

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
   class Gui : public dev::CaenDigitizerGui,
               public virtual Device {

      Q_OBJECT

   public:
      /// Constructor
      Gui( QWidget* parent = 0, Qt::WindowFlags flags = 0 );

      /// Read the device configuration from a binary file
      virtual StatusCode readConfig( QIODevice& dev );
      /// Read the device configuration from an XML file
      virtual StatusCode readConfig( const QDomElement& node );

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
      std::unique_ptr< QScrollArea > m_scrollArea;
      /// Main widget that is scrolled
      std::unique_ptr< QWidget > m_scrollWidget;

      /// Image of the digitizer itself
      std::unique_ptr< QLabel > m_image;
      /// Text label at the top
      std::unique_ptr< QLabel > m_topLabel;

      /// Box for the connection parameters
      std::unique_ptr< QGroupBox > m_connectionBox;
      /// Description for conn. mode setting
      std::unique_ptr< QLabel > m_connModeLabel;
      /// Connection mode chooser
      std::unique_ptr< QComboBox > m_connMode;
      /// Description for conn. link setting
      std::unique_ptr< QLabel > m_connLinkLabel;
      /// Connection link chooser
      std::unique_ptr< QSpinBox > m_connLink;

      /// Box for the trigger settings
      std::unique_ptr< QGroupBox > m_triggerBox;
      /// Setting for enabling trigger overlaps
      std::unique_ptr< QCheckBox > m_trigOvlpEnabledWidget;
      /// External trigger setting
      std::unique_ptr< QCheckBox > m_extTrigEnabledWidget;
      /// External trigger forward setting
      std::unique_ptr< QCheckBox > m_extTrigOutEnabledWidget;
      /// GPO impedance level setting
      std::unique_ptr< QCheckBox > m_highImpedanceGPOWidget;
      /// Description for post trigger fraction
      std::unique_ptr< QLabel > m_postTrigPercentageLabel;
      /// Post trigger fraction setting
      std::unique_ptr< QSpinBox > m_postTrigPercentageWidget;
      /// Description of the trigger mode setting
      std::unique_ptr< QLabel > m_trigModeLabel;
      /// Trigger mode chooser
      std::unique_ptr< QComboBox > m_trigModeWidget;
      /// Description for front panel signal type
      std::unique_ptr< QLabel > m_signalTypeLabel;
      /// Front panel signal type setting
      std::unique_ptr< QComboBox > m_signalTypeWidget;

      /// Box for the acquisition settings
      std::unique_ptr< QGroupBox > m_acquisitionBox;
      /// Pattern generation setting
      std::unique_ptr< QCheckBox > m_patGenEnabledWidget;
      /// RAW ntuple save setting
      std::unique_ptr< QCheckBox > m_saveRawNtupleWidget;
      /// Description of the acquisition mode setting
      std::unique_ptr< QLabel > m_acqModeLabel;
      /// Acquisition mode chooser
      std::unique_ptr< QComboBox > m_acqModeWidget;
      /// Description of the gate mode setting
      std::unique_ptr< QLabel > m_gateModeLabel;
      /// Gate mode chooser
      std::unique_ptr< QComboBox > m_gateModeWidget;
      /// Description of buffer mode setting
      std::unique_ptr< QLabel > m_bufferModeLabel;
      /// Buffer mode chooser
      std::unique_ptr< QComboBox > m_bufferModeWidget;
      /// Description of the clock source setting
      std::unique_ptr< QLabel > m_clockSourceLabel;
      /// Clock source chooser
      std::unique_ptr< QComboBox > m_clockSourceWidget;
      /// Description of the event counting mode setting
      std::unique_ptr< QLabel > m_evCountModeLabel;
      /// Event counting mode chooser
      std::unique_ptr< QComboBox > m_evCountModeWidget;

      /// Box for the signal reconstruction settings
      std::unique_ptr< QGroupBox > m_recoBox;
      /// Description of the CFD fraction setting
      std::unique_ptr< QLabel > m_cfdFractionLabel;
      /// Digital CFD fraction setting
      std::unique_ptr< QDoubleSpinBox > m_cfdFractionWidget;
      /// Description of the CFD delay setting
      std::unique_ptr< QLabel > m_cfdDelayLabel;
      /// Digital CFD delay setting
      std::unique_ptr< QSpinBox > m_cfdDelayWidget;
      /// Description of the CFD length setting
      std::unique_ptr< QLabel > m_cfdLengthLabel;
      /// Digital CFD length setting
      std::unique_ptr< QSpinBox > m_cfdLengthWidget;
      /// Description of the Gaussian smoothing width setting
      std::unique_ptr< QLabel > m_gaussSmoothWidthLabel;
      /// Gaussian smoothing width setting
      std::unique_ptr< QDoubleSpinBox > m_gaussSmoothWidthWidget;

      /// The graphical representation of channel groups
      std::unique_ptr< GroupGui > m_ggroups[ NUMBER_OF_GROUPS ];

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class Gui

} // namespace dt5740

#endif // CDA_DEVICES_DT5740_GUI_H
