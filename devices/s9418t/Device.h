// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_S9418T_DEVICE_H
#define CDA_DEVICES_S9418T_DEVICE_H

// Qt include(s):
#include <QtCore/QCoreApplication>

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

namespace s9418t {

   /**
    *  @short Base class for the S9418T devices
    *
    *         This class stores the configuration of a Silena 9418T
    *         VME device and takes care of reading and writing this
    *         configuration in all the supported formats.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class Device : virtual public dev::Device {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( s9418t::Device )

   public:
      /// Default constructor
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

      /// The type of the device
      virtual QString deviceName() const { return "S9418T"; }

      /// Get the slot of the device in the CAMAC crate, or address of a VME device
      virtual unsigned int getID() const;
      /// Set the slot of the device in the CAMAC crate, or address of the VME device
      virtual void setID( unsigned int value );

   protected:
      /// Clear the configuration of the device
      void clear();

      /// Number of inputs on the device
      static const int NUMBER_OF_CHANNELS = 32;

      /// Module operating mode types
      enum OpMode {
         CommonStart = 0, ///< All channels are started with a common signal
         CommonStop  = 1  ///< All channels are stopped with a common signal
      }; // enum OpMode

      unsigned int m_address; ///< Base address of the module
      short m_vsn; ///< Virtual Station Number
      short m_calib; ///< Calibration test pulse amplitude
      short m_range; ///< Range of the measurement
      short m_offset; ///< Time offset of the module
      OpMode m_opMode; ///< Operating mode of the module

      /// Configuration of the input channels of the device
      ChannelConfig* m_channels[ NUMBER_OF_CHANNELS ];

   private:
      mutable msg::Logger m_logger; ///< Message logger object

   }; // class Device

} // namespace s9418t

#endif // CDA_DEVICES_S9418T_DEVICE_H
