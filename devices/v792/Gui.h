// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_V792_GUI_H
#define CDA_DEVICES_V792_GUI_H

// System include(s):
#include <memory>
#include <array>

// Qt include(s):
#include <QWidget>
#include <QScrollArea>
#include <QLabel>
#include <QGroupBox>
#include <QLineEdit>
#include <QCheckBox>

// CDA include(s):
#include "device/CaenVmeGui.h"
#include "msg/Logger.h"

// Local include(s):
#include "Device.h"
#include "ChannelGui.h"

namespace v792 {

   /// Class used to interactively configure the data collection from a V792
   ///
   /// This class is used to configure the data collection properties of a
   /// V792 device.
   ///
   /// @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
   ///
   /// $Revision$
   /// $Date$
   ///
   class Gui : public dev::CaenVmeGui,
         public virtual Device {

      Q_OBJECT

   public:
      /// Standard Qt widget constructor
      Gui( QWidget* parent = 0, Qt::WindowFlags flags = Qt::WindowFlags() );

      /// Read the device configuration from a binary file
      virtual StatusCode readConfig( QIODevice& dev );
      /// Read the device configuration from an XML file
      virtual StatusCode readConfig( const QDomElement& node );

   private slots:
      /// Slot for VME address changes
      void vmeAddressSlot( const QString& value );
      /// Slot for zero suppression changes
      void zeroSuppressionSlot( bool value );
      /// Slot for overflow suppression changes
      void overflowSuppressionSlot( bool value );
      /// Slot for valid suppression changes
      void validSuppressionSlot( bool value );

      /// Slot handling changes to channel enablement
      void channelEnabledSlot( int channel, bool on );
      /// Slot handling changes to channel names
      void nameChangedSlot( int channel, const QString& text );
      /// Slot handling changes to histogram bin numbers
      void nbinsChangedSlot( int channel, int nbins );
      /// Slot handling changes to histogram lower bounds
      void lowerBoundChangedSlot( int channel, double value );
      /// Slot handling changes to histogram upper bounds
      void upperBoundChangedSlot( int channel, double value );
      /// Slot handling changes to channel readout thresholds
      void thresholdChangedSlot( int channel, int value );

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

      /// Device wide settings box
      std::unique_ptr< QGroupBox > m_deviceSettingsBox;
      /// Label for the VME address of the device
      std::unique_ptr< QLabel > m_vmeAddressLabel;
      /// Editor for the VME address of the device
      std::unique_ptr< QLineEdit > m_vmeAddressEdit;
      /// Zero suppression editor
      std::unique_ptr< QCheckBox > m_zeroSuppressionEdit;
      /// Overflow suppression editor
      std::unique_ptr< QCheckBox > m_overflowSuppressionEdit;
      /// Valid suppression editor
      std::unique_ptr< QCheckBox > m_validSuppressionEdit;

      /// Channel settings box
      std::unique_ptr< QGroupBox > m_channelSettingsBox;
      /// Label for the channel names
      std::unique_ptr< QLabel > m_channelNameLabel;
      /// Label for the channel histogram number of bins
      std::unique_ptr< QLabel > m_channelNBinsLabel;
      /// Label for the channel histogram lower bounds
      std::unique_ptr< QLabel > m_channelLowerBoundLabel;
      /// Label for the channel histogram upper bounds
      std::unique_ptr< QLabel > m_channelUpperBoundLabel;
      /// The graphical representation of the input channels
      std::array< std::unique_ptr< ChannelGui >,
                  NUMBER_OF_CHANNELS > m_gchannels;

      /// A message logging object
      mutable msg::Logger m_logger;

   }; // class Gui

} // namespace v792

#endif // CDA_DEVICES_V792_GUI_H
