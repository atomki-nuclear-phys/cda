// Dear emacs, this is -*- c++ -*-
// $Id: Device.h 30 2008-07-10 13:49:27Z krasznaa $
#ifndef CDA_DEVICES_AD2249A_DEVICE_H
#define CDA_DEVICES_AD2249A_DEVICE_H

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/Device.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "device/Device.h"
#   include "msg/Logger.h"
#endif

// Local include(s):
#include "ChannelConfig.h"

namespace ad2249a {

   /**
    *  @short Base class for the AD2249A devices
    *
    *         This class stores the configuration of a AD2249A CAMAC device
    *         and takes care of reading and writing this configuration
    *         in all the necessary formats.
    */
   class Device : virtual public dev::Device {

   public:
      /// Constructor
      Device();
      /// Destructor
      ~Device();

      /// Function reading the configuration in binary format
      virtual bool readConfig( QIODevice* dev );
      /// Function writing the configuration in binary format
      virtual bool writeConfig( QIODevice* dev ) const;

      /// Function reading the configuration in XML format
      virtual bool readConfig( const QDomElement& node );
      /// Function writing the configuration in XML format
      virtual bool writeConfig( QDomElement& node ) const;

      /// The type of the child class
      virtual QString type() const { return "AD2249A"; }

      /// Get the slot of the device in the CAMAC crate
      virtual int getSlot() const;
      /// Set the slot of the device in the CAMAC crate
      virtual void setSlot( int value );

   protected:
      /// Clear the configuration of the device
      void clear();

      /// Number of inputs on the device
      static const int NUMBER_OF_SUBADDRESSES = 12;

      int            m_slot; ///< Slot of the device in the CAMAC crate
      bool           m_generateLam; ///< Generate LAM signal at readout
      /// Configuration of the input channels of the device
      ChannelConfig* m_channels[ NUMBER_OF_SUBADDRESSES ];
      mutable qint32  m_number_of_channels;

   private:
      mutable msg::Logger m_logger; ///< Message logger object

   }; // class Device

} // namespace ad2249a

#endif // CDA_DEVICES_AD2249A_DEVICE_H
