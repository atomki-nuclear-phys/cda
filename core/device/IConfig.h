// Dear emacs, this is -*- c++ -*-
#ifndef CDA_CORE_DEVICE_ICONFIG_H
#define CDA_CORE_DEVICE_ICONFIG_H

// Qt include(s):
#include <QtGlobal>

// Local include(s):
#include "../common/StatusCode.h"

// Disable a warning about diamond inheritance trees
#ifdef Q_CC_MSVC
#pragma warning(disable : 4250)
#endif  // Q_CC_MSVC

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS(QIODevice)
QT_FORWARD_DECLARE_CLASS(QDomElement)

/**
 *  @short Namespace holding the device (base) classes
 *
 *         All the classes that should be used as base classes for
 *         the implementation of the CAMAC devices, are stored in this
 *         namespace.
 *
 *         In addition classes providing core functionalities for handling
 *         the device objects are also placed in this namespace.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */
namespace dev {

//
// Make sure that the following Qt classes are available in the
// DEV namespace even if Qt has been built in an arbitrary
// namespace:
//
using QT_PREPEND_NAMESPACE(QIODevice);
using QT_PREPEND_NAMESPACE(QDomElement);

/**
 *  @short Base interface for configuring a device
 *
 *         Each device has to be able to read and write its
 *         configuration. This interface class defines the functions
 *         that have to be implemented in each device class to achieve
 *         this functionality.
 *
 *         There are two pairs of functions, as the configuration has to
 *         be sent over binary connections (TCP for instance) and also
 *         written out to a user readable format.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */
class IConfig {

public:
   /// Destructor
   virtual ~IConfig() {}

   /// Function reading the configuration in binary format
   /**
    * This function can be used to read the configuration of a device
    * from a generic QIODevice. Since practically all data handling
    * classes inherit from QIODevice in Qt, it can be used with a wide
    * variety of objects.
    *
    * @param dev Pointer to the device providing the configuration
    * @return StatusCode with the outcome of the operation
    */
   virtual StatusCode readConfig(QIODevice& dev) = 0;
   /// Function writing the configuration in binary format
   /**
    * This function can be used to write the configuration of a device
    * to a generic QIODevice. Since practically all data handling
    * classes inherit from QIODevice in Qt, it can be used with a wide
    * variety of objects.
    *
    * @param dev Pointer to the device where the configuration is to
    *            be written
    * @return StatusCode with the outcome of the operation
    */
   virtual StatusCode writeConfig(QIODevice& dev) const = 0;

   /// Function reading the configuration in XML format
   /**
    * This function can be used to read the configuration from an XML
    * DOM node. It is used when reading the configuration of the
    * device from a text file.
    *
    * @param node The element type node holding the configuration of this
    *             device
    * @return StatusCode with the outcome of the operation
    */
   virtual StatusCode readConfig(const QDomElement& node) = 0;
   /// Function writing the configuration in XML format
   /**
    * This function can be used to write the configuration of the device
    * to an empty XML DOM node. It is used when writing the configuration
    * of the device to a text file.
    *
    * @param node An empty element type node where the configuration is
    *             to be written
    * @return StatusCode with the outcome of the operation
    */
   virtual StatusCode writeConfig(QDomElement& node) const = 0;

};  // class IConfig

}  // namespace dev

#endif  // CDA_CORE_DEVICE_ICONFIG_H
