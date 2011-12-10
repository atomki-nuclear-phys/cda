// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_DT5740_CHANNELCONFIG_H
#define CDA_DEVICES_DT5740_CHANNELCONFIG_H

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/Config.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "device/Config.h"
#   include "msg/Logger.h"
#endif

namespace dt5740 {

   // Forward declaration(s):
   class Device;

   /**
    *  @short Class describing *the* one channel of an AD1000 module
    *
    *         The one channel of an AD1000 module has a number of properties.
    *         All these properties are stored in such objects.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class ChannelConfig : virtual public dev::Config {

      /// Declare the Device class a friend of this class
      friend class dt5740::Device;

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( dt5740::ChannelConfig )

   public:
      /// Constructor
      ChannelConfig();

      /// Function reading the configuration in binary format
      virtual bool readConfig( QIODevice* dev );
      /// Function writing the configuration in binary format
      virtual bool writeConfig( QIODevice* dev ) const;

      /// Function reading the configuration in XML format
      virtual bool readConfig( const QDomElement& node );
      /// Function writing the configuration in XML format
      virtual bool writeConfig( QDomElement& node ) const;

      /// Get the number of channels for the monitoring histogram
      int            getNumberOfChannels() const;
      /// Get the lower bound of the monitoring histogram
      double         getLowerBound() const;
      /// Get the upper bound of the monitoring histogram
      double         getUpperBound() const;
      /// Get the name of the channel
      const QString& getName() const;
      /// Get the channel's number
      int            getChannelNumber() const;

      /// Set the number of channels for the monitoring histogram
      void setNumberOfChannels( int value );
      /// Set the lower bound of the monitoring histogram
      void setLowerBound( double value );
      /// Set the upper bound of the monitoring histogram
      void setUpperBound( double value );
      /// Set the name of the channel
      void setName( const QString& value );
      /// Set the channel's number
      void setChannelNumber( int number );

      /// Print the configuration of the channel
      void printConfig( msg::Level level = msg::INFO ) const;

   private:
      /// Clear the channel's configuration
      void clear();

      int     m_numberOfChannels; ///< Number of channels in the created histogram
      double  m_lowerBound;       ///< Lower bound of the created histogram
      double  m_upperBound;       ///< Upper bound of the created histogram
      QString m_name;             ///< Name of the channel
      int     m_channelNumber;    ///< The channel's number in the device

      mutable msg::Logger m_logger; ///< Logger member

   }; // class ChannelConfig

} // namespace dt5740

#endif // CDA_DEVICES_DT5740_CHANNELCONFIG_H
