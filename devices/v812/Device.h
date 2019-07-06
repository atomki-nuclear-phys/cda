// Dear emacs, this is -*- c++ -*-
#ifndef CDA_DEVICES_V812_DEVICE_H
#define CDA_DEVICES_V812_DEVICE_H

// System include(s).
#include <memory>
#include <array>

// Qt include(s).
#include <QtCore/QCoreApplication>

// CDA include(s).
#include "device/IDevice.h"
#include "msg/Logger.h"

// Local include(s).
#include "ChannelConfig.h"

namespace v812 {

   /// Class holding the configuration of a V812 CAEN VME CFD
   ///
   /// This is used as the base class for all the other device classes in the
   /// plugin. Storing, writing, and reading the configuration of a V812
   /// device.
   ///
   /// @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
   ///
   class Device : public virtual dev::IDevice {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( v812::Device )

   public:
      /// Default constructor
      Device() = default;

      /// @name Functions inherited from @c dev::IDevice
      /// @{

      /// Function reading the configuration in binary format
      virtual StatusCode readConfig( QIODevice& dev );
      /// Function writing the configuration in binary format
      virtual StatusCode writeConfig( QIODevice& dev ) const;

      /// Function reading the configuration in XML format
      virtual StatusCode readConfig( const QDomElement& node );
      /// Function writing the configuration in XML format
      virtual StatusCode writeConfig( QDomElement& node ) const;

      /// The type of the child class
      virtual const QString& deviceName() const;

      /// ID (VME address) of the device
      virtual unsigned int getID() const;
      /// Sets the ID (VME address) of the device
      virtual void setID( unsigned int value );

      /// @}

      /// Clear the configuration of the device
      void clear();

   protected:
      /// Number of channels in the device
      static const size_t NUMBER_OF_CHANNELS = 16;

      /// The VME address of the device
      unsigned int m_vmeAddress = 0x0;
      /// Output width for channels 0 - 7
      uint8_t m_outputWidth1 = 0;
      /// Output width for channels 8 - 15
      uint8_t m_outputWidth2 = 0;
      /// Dead time for channels 0 - 7
      uint8_t m_deadTime1 = 0;
      /// Dead time for channels 8 - 15
      uint8_t m_deadTime2 = 0;
      /// Majority threshold
      uint8_t m_majorityThreshold = 1;

      /// Configuration of the channels of the device
      std::array< std::unique_ptr< ChannelConfig >,
                  NUMBER_OF_CHANNELS > m_channels;

      /// A message logger object
      mutable msg::Logger m_logger{ "v812::Device" };

   }; // class Device

} // namespace v812

#endif // CDA_DEVICES_V812_DEVICE_H
