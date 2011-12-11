// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_DT5740_DEVICE_H
#define CDA_DEVICES_DT5740_DEVICE_H

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/Device.h"
#   include "cdacore/caen/Digitizer.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "device/Device.h"
#   include "caen/Digitizer.h"
#   include "msg/Logger.h"
#endif

// Local include(s):
#include "ChannelConfig.h"
#include "GroupConfig.h"

namespace dt5740 {

   /**
    *  @short Base class for the DT5740 devices
    *
    *         This class stores the configuration of a DT5740 digitizer
    *         and takes care of reading and writing this configuration
    *         in all the necessary formats.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class Device : virtual public dev::Device {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( dt5740::Device )

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
      virtual QString deviceName() const { return "DT5740"; }

      /// ID of the USB connection
      virtual unsigned int getID() const;
      /// Sets the ID of the USB connection (not used)
      virtual void setID( unsigned int value );

   protected:
      /// Clear the configuration of the device
      void clear();

      /// Number of channel groups handled by the device
      static const int NUMBER_OF_GROUPS = 4;
      /// Total number of input channels on the device
      static const int NUMBER_OF_CHANNELS = 32;

      /// Configuration of the input channels
      ChannelConfig* m_channels[ NUMBER_OF_CHANNELS ];
      /// Configuration of the channel groups
      GroupConfig m_groups[ NUMBER_OF_GROUPS ];

      /// Connection protocol to access the device
      caen::Digitizer::ConnectionType m_connType;
      /// Link number (0 if only USB device)
      int m_linkNumber;

   private:
      mutable msg::Logger m_logger; ///< Message logger object

   }; // class Device

} // namespace dt5740

#endif // CDA_DEVICES_DT5740_DEVICE_H
