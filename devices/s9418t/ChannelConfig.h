// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_S9418T_CHANNELCONFIG_H
#define CDA_DEVICES_S9418T_CHANNELCONFIG_H

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/IConfig.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "device/IConfig.h"
#   include "msg/Logger.h"
#endif

namespace s9418t {

   /**
    *  @short Class describing one channel of an S9418T module
    *
    *         Each channel of an S9418T module has a number of properties.
    *         All these properties are stored in such objects.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class ChannelConfig : virtual public dev::IConfig {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( s9418t::ChannelConfig )

   public:
      /// Constructor
      ChannelConfig();

      /// Function reading the configuration in binary format
      virtual bool readConfig( QIODevice& dev );
      /// Function writing the configuration in binary format
      virtual bool writeConfig( QIODevice& dev ) const;

      /// Function reading the configuration in XML format
      virtual bool readConfig( const QDomElement& node );
      /// Function writing the configuration in XML format
      virtual bool writeConfig( QDomElement& node ) const;

      /// Get the channel number of this channel
      int            getChannel() const;
      /// Get the number of channels for the monitoring histogram
      int            getNumberOfChannels() const;
      /// Get the lower bound of the monitoring histogram
      double         getLowerBound() const;
      /// Get the upper bound of the monitoring histogram
      double         getUpperBound() const;
      /// Get the name of the channel
      const QString& getName() const;
      /// Get the channel's lower threshold
      short          getThreshold() const;

      /// Set the subaddress of this channel
      void setChannel( int value );
      /// Set the number of channels for the monitoring histogram
      void setNumberOfChannels( int value );
      /// Set the lower bound of the monitoring histogram
      void setLowerBound( double value );
      /// Set the upper bound of the monitoring histogram
      void setUpperBound( double value );
      /// Set the name of the channel
      void setName( const QString& value );
      /// Set the channel's lower threshold
      void setThreshold( short value );

      /// Print the configuration of the channel
      void printConfig( msg::Level level = msg::INFO ) const;

   private:
      /// Clear the channel's configuration
      void clear();

      int     m_channel;          ///< Channel number of this channel
      int     m_numberOfChannels; ///< Number of channels in the created histogram
      double  m_lowerBound;       ///< Lower bound of the created histogram
      double  m_upperBound;       ///< Upper bound of the created histogram
      QString m_name;             ///< Name of the channel
      short   m_threshold;        ///< Zero suppression threshold

      mutable msg::Logger m_logger; ///< Logger member

   }; // class ChannelConfig

} // namespace s9418t

#endif // CDA_DEVICES_S9418T_CHANNELCONFIG_H
