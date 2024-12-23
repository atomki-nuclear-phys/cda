// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_T2228A_DEVICE_H
#define CDA_DEVICES_T2228A_DEVICE_H

// System include(s):
#include <memory>

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#include "device/IDevice.h"
#include "msg/Logger.h"

// Local include(s):
#include "ChannelConfig.h"

namespace t2228a {

/**
 *  @short Base class for the T2228A devices
 *
 *         This class stores the configuration of a T2228A CAMAC device
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
   Q_DECLARE_TR_FUNCTIONS(t2228a::Device)

public:
   /// Constructor
   Device();

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

   /// Get the slot of the device in the CAMAC crate
   virtual unsigned int getID() const;
   /// Set the slot of the device in the CAMAC crate
   virtual void setID(unsigned int value);

protected:
   /// Clear the configuration of the device
   void clear();

   /// Number of inputs on the device
   static const int NUMBER_OF_SUBADDRESSES = 8;

   unsigned int m_slot;  ///< Slot of the device in the CAMAC crate
   bool m_generateLam;   ///< Generate LAM signal at readout
   /// Configuration of the input channels of the device
   std::unique_ptr<ChannelConfig> m_channels[NUMBER_OF_SUBADDRESSES];

private:
   mutable msg::Logger m_logger;  ///< Message logger object

};  // class Device

}  // namespace t2228a

#endif  // CDA_DEVICES_T2228A_DEVICE_H
