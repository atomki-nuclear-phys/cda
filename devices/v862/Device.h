// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_V862_DEVICE_H
#define CDA_DEVICES_V862_DEVICE_H

// System include(s):
#include <memory>

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#include "device/IDevice.h"
#include "msg/Logger.h"

// Local include(s):
#include "ChannelConfig.h"

namespace v862 {

   /// Class holding the configuration of a V862 CAEN VME QDC
   ///
   /// This is used as the base class for all the other device classes in the
   /// plugin. Storing, writing, and reading the configuration of a V862
   /// device.
   ///
   /// @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
   ///
   /// $Revision$
   /// $Date$
   ///
   class Device : public virtual dev::IDevice {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( v862::Device )

   public:
      /// Default constructor
      Device();

      /// @name Functions inherited from dev::IDevice
      /// @{

      /// Function reading the configuration in binary format
      virtual bool readConfig( QIODevice& dev );
      /// Function writing the configuration in binary format
      virtual bool writeConfig( QIODevice& dev ) const;

      /// Function reading the configuration in XML format
      virtual bool readConfig( const QDomElement& node );
      /// Function writing the configuration in XML format
      virtual bool writeConfig( QDomElement& node ) const;

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
      static const size_t NUMBER_OF_CHANNELS = 32;

      /// The VME address of the device
      unsigned int m_vmeAddress;
      /// Flag enabling/disabling zero suppression in the readout
      bool m_zeroSuppressionEnabled;
      /// Flag enabling/disabling overflow suppression in the readout
      bool m_overflowSuppressionEnabled;
      /// Flag enabling/disabling valid data suppression in the readout
      bool m_validSuppressionEnabled;

      /// Configuration of the channels of the device
      std::unique_ptr< ChannelConfig > m_channels[ NUMBER_OF_CHANNELS ];

      /// A message logger object
      mutable msg::Logger m_logger;

   }; // class Device

} // namespace v862

#endif // CDA_DEVICES_V862_DEVICE_H
