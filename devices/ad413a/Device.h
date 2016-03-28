// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_AD413A_DEVICE_H
#define CDA_DEVICES_AD413A_DEVICE_H

// System include(s):
#include <memory>

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#include "device/IDevice.h"
#include "msg/Logger.h"

// Local include(s):
#include "ChannelConfig.h"

namespace ad413a {

   /**
    *  @short Base class for the AD413A devices
    *
    *         This class stores the configuration of a AD413A CAMAC device
    *         and takes care of reading and writing this configuration
    *         in all the necessary formats.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class Device : virtual public dev::IDevice {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( ad413a::Device )

   public:
      /// Constructor
      Device();

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

      /// Get the slot of the device in the CAMAC crate
      virtual unsigned int getID() const;
      /// Set the slot of the device in the CAMAC crate
      virtual void setID( unsigned int value );

   protected:
      /// Clear the configuration of the device
      void clear();

      /// Number of inputs on the device
      static const int NUMBER_OF_SUBADDRESSES = 4;

      unsigned int m_slot; ///< Slot of the device in the CAMAC crate
      bool         m_generateLam; ///< Generate LAM signal at readout
      short        m_gate; ///< Type of gate that should be used

      /// Configuration of the input channels of the device
      std::unique_ptr< ChannelConfig > m_channels[ NUMBER_OF_SUBADDRESSES ];

   private:
      mutable msg::Logger m_logger; ///< Message logger object

   }; // class Device

} // namespace ad413a

#endif // CDA_DEVICES_AD413A_DEVICE_H
