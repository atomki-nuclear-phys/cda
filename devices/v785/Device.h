// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_V785_DEVICE_H
#define CDA_DEVICES_V785_DEVICE_H

// System include(s):
#include <array>
#include <memory>

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#include "caen/VmeDevice.h"
#include "device/IDevice.h"
#include "event/Fragment.h"
#include "msg/Logger.h"

// Local include(s):
#include "ChannelConfig.h"

namespace v785 {

/// Class holding the configuration of a V785 CAEN VME ADC
///
/// This is used as the base class for all the other device classes in the
/// plugin. Storing, writing, and reading the configuration of a V785
/// device.
///
/// @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
///
/// $Revision$
/// $Date$
///
class Device : public virtual dev::IDevice {

   // To get the tr() function:
   Q_DECLARE_TR_FUNCTIONS(v785::Device)

public:
   /// Default constructor
   Device();

   /// @name Functions inherited from dev::IDevice
   /// @{

   /// Function reading the configuration in binary format
   virtual StatusCode readConfig(QIODevice& dev);
   /// Function writing the configuration in binary format
   virtual StatusCode writeConfig(QIODevice& dev) const;

   /// Function reading the configuration in XML format
   virtual StatusCode readConfig(const QDomElement& node);
   /// Function writing the configuration in XML format
   virtual StatusCode writeConfig(QDomElement& node) const;

   /// The type of the child class
   virtual const QString& deviceName() const;

   /// ID (VME address) of the device
   virtual unsigned int getID() const;
   /// Sets the ID (VME address) of the device
   virtual void setID(unsigned int value);

   /// @}

   /// Clear the configuration of the device
   void clear();

protected:
   /// Number of channels in the device
   static const size_t NUMBER_OF_CHANNELS = 32;

   /// Structure describing the data from a single channel
   class ChannelData {

   public:
      /// Constructor from a caen::VmeDevice::DataWord object
      ChannelData(const caen::VmeDevice::DataWord& dw);
      /// Constructor from one word in an event fragment
      ChannelData(ev::Fragment::Payload_t::value_type data);
      /// Automatic conversion to a word in an event fragment
      operator ev::Fragment::Payload_t::value_type() const { return m_data; }

      /// Get the channel number
      int channel() const;
      /// Get the qdc data
      int data() const;
      /// Get the under threshold bit
      bool underThreshold() const;
      /// Get the overflow bit
      bool overflow() const;

   private:
      uint32_t m_data;  ///< Encoded data word

   };  // class ChannelData

   /// The VME address of the device
   unsigned int m_vmeAddress;
   /// Flag enabling/disabling zero suppression in the readout
   bool m_zeroSuppressionEnabled;
   /// Flag enabling/disabling overflow suppression in the readout
   bool m_overflowSuppressionEnabled;
   /// Flag enabling/disabling valid data suppression in the readout
   bool m_validSuppressionEnabled;

   /// Configuration of the channels of the device
   std::array<std::unique_ptr<ChannelConfig>, NUMBER_OF_CHANNELS> m_channels;

   /// A message logger object
   mutable msg::Logger m_logger;

};  // class Device

}  // namespace v785

#endif  // CDA_DEVICES_V785_DEVICE_H
