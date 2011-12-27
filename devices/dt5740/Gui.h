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
QT_FORWARD_DECLARE_CLASS( QDoubleSpinBox )

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

      QGroupBox* m_triggerBox;               ///< Box for the trigger settings
      QCheckBox* m_trigOvlpEnabledWidget;    ///< Setting for enabling trigger overlaps
      QCheckBox* m_extTrigEnabledWidget;     ///< External trigger setting
      QCheckBox* m_extTrigOutEnabledWidget;  ///< External trigger forward setting
      QCheckBox* m_highImpedanceGPOWidget;   ///< GPO impedance level setting
      QLabel*    m_postTrigPercentageLabel;  ///< Description for post trigger fraction
      QSpinBox*  m_postTrigPercentageWidget; ///< Post trigger fraction setting
      QLabel*    m_trigModeLabel;            ///< Description of the trigger mode setting
      QComboBox* m_trigModeWidget;           ///< Trigger mode chooser
      QLabel*    m_signalTypeLabel;          ///< Description for front panel signal type
      QComboBox* m_signalTypeWidget;         ///< Front panel signal type setting

      QGroupBox* m_acquisitionBox;      ///< Box for the acquisition settings
      QCheckBox* m_patGenEnabledWidget; ///< Pattern generation setting
      QCheckBox* m_saveRawNtupleWidget; ///< RAW ntuple save setting
      QLabel*    m_acqModeLabel;        ///< Description of the acquisition mode setting
      QComboBox* m_acqModeWidget;       ///< Acquisition mode chooser
      QLabel*    m_gateModeLabel;       ///< Description of the gate mode setting
      QComboBox* m_gateModeWidget;      ///< Gate mode chooser
      QLabel*    m_bufferModeLabel;     ///< Description of buffer mode setting
      QComboBox* m_bufferModeWidget;    ///< Buffer mode chooser
      QLabel*    m_clockSourceLabel;    ///< Description of the clock source setting
      QComboBox* m_clockSourceWidget;   ///< Clock source chooser
      QLabel*    m_evCountModeLabel;    ///< Description of the event counting mode setting
      QComboBox* m_evCountModeWidget;   ///< Event counting mode chooser

      QGroupBox*      m_recoBox;           ///< Box for the signal reconstruction settings
      QLabel*         m_cfdFractionLabel;  ///< Description of the CFD fraction setting
      QDoubleSpinBox* m_cfdFractionWidget; ///< Digital CFD fraction setting
      QLabel*         m_cfdDelayLabel;     ///< Description of the CFD delay setting
      QSpinBox*       m_cfdDelayWidget;    ///< Digital CFD delay setting
      QLabel*         m_cfdLengthLabel;    ///< Description of the CFD length setting
      QSpinBox*       m_cfdLengthWidget;   ///< Digital CFD length setting

      /// The graphical representation of channel groups
      GroupGui* m_ggroups[ NUMBER_OF_GROUPS ];

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class Gui

} // namespace dt5740

#endif // CDA_DEVICES_DT5740_GUI_H
