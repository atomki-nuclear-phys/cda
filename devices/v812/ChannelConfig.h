// Dear emacs, this is -*- c++ -*-
#ifndef CDA_DEVICES_V812_CHANNELCONFIG_H
#define CDA_DEVICES_V812_CHANNELCONFIG_H

// CDA include(s).
#include "device/IConfig.h"
#include "msg/Logger.h"

// Qt include(s).
#include <QtCore/QCoreApplication>
#include <QtCore/QString>

namespace v812 {

   /// Class describing one channel of a V812 CFD
   ///
   /// Each channel of a V812 module has a number of properties. All these
   /// properties are stored in such objects.
   ///
   /// @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
   ///
   class ChannelConfig : virtual public dev::IConfig {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( v812::ChannelConfig )

   public:
      /// Constructor
      ChannelConfig();

      /// @name Function(s) inherited from @c dev::IConfig
      /// @{

      /// Function reading the configuration in binary format
      virtual StatusCode readConfig( QIODevice& dev );
      /// Function writing the configuration in binary format
      virtual StatusCode writeConfig( QIODevice& dev ) const;

      /// Function reading the configuration in XML format
      virtual StatusCode readConfig( const QDomElement& node );
      /// Function writing the configuration in XML format
      virtual StatusCode writeConfig( QDomElement& node ) const;

      /// @}

      /// Get the channel number
      int getChannel() const;
      /// Set the channel number
      void setChannel( int value );

      /// Get the discriminator threshold for the channel
      uint16_t getThreshold() const;
      /// Set the discriminator threshold for the channel
      void setThreshold( uint16_t value );

      /// Print the configuration of the channel
      void printConfig( msg::Level level = msg::INFO ) const;

   private:
      /// Clear the channel's configuration
      void clear();

      /// The channel's number
      int m_channel;
      /// Discriminator threshold for the channel
      uint16_t m_threshold;

      /// Logger object
      mutable msg::Logger m_logger;

   }; // class ChannelConfig

} // namespace v812

#endif // CDA_DEVICES_V812_CHANNELCONFIG_H
