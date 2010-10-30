// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_AD413A_DEVICE_H
#define CDA_DEVICES_AD413A_DEVICE_H

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

namespace ad413a {

   /**
    *  @short Base class for the AD413A devices
    *
    *         This class stores the configuration of a AD413A CAMAC device
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
      virtual QString type() const { return "AD413A"; }

      /// Get the slot of the device in the CAMAC crate
      virtual int getSlot() const;
      /// Set the slot of the device in the CAMAC crate
      virtual void setSlot( int value );

   protected:
      /// Clear the configuration of the device
      void clear();

      /// Number of inputs on the device
      static const int NUMBER_OF_SUBADDRESSES = 4;

      int            m_slot; ///< Slot of the device in the CAMAC crate
      bool           m_generateLam; ///< Generate LAM signal at readout
      short          m_gate; ///< Type of gate that should be used

      /// Configuration of the input channels of the device
      ChannelConfig* m_channels[ NUMBER_OF_SUBADDRESSES ];

   private:
      mutable msg::Logger m_logger; ///< Message logger object

   }; // class Device

} // namespace ad413a

#endif // CDA_DEVICES_AD413A_DEVICE_H
