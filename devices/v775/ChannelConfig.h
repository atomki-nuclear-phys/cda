// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_V775_CHANNELCONFIG_H
#define CDA_DEVICES_V775_CHANNELCONFIG_H

// Qt include(s):
#include <QtCore/QCoreApplication>
#include <QtCore/QString>

// CDA include(s):
#include "device/IConfig.h"
#include "msg/Logger.h"

namespace v775 {

   /**
    *  @short Class describing one channel of a V775 TDC module
    *
    *         Each channel of a V775 module has a number of properties.
    *         All these properties are stored in such objects.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class ChannelConfig : virtual public dev::IConfig {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( v775::ChannelConfig )

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

      /// Get the channel (subaddress) number
      int            getChannel() const;
      /// Get the number of bins for the monitoring histogram
      int            getNBins() const;
      /// Get the lower bound of the monitoring histogram
      double         getLowerBound() const;
      /// Get the upper bound of the monitoring histogram
      double         getUpperBound() const;
      /// Get the name of the channel
      const QString& getName() const;
      /// Get the readout threshold for the channel
      int            getThreshold() const;

      /// Set the channel (subaddress) number
      void setChannel( int value );
      /// Set the number of bins for the monitoring histogram
      void setNBins( int value );
      /// Set the lower bound of the monitoring histogram
      void setLowerBound( double value );
      /// Set the upper bound of the monitoring histogram
      void setUpperBound( double value );
      /// Set the name of the channel
      void setName( const QString& value );
      /// Set the readout threshold for the channel
      void setThreshold( int value );

      /// Print the configuration of the channel
      void printConfig( msg::Level level = msg::INFO ) const;

   private:
      /// Clear the channel's configuration
      void clear();

      int     m_channel;    ///< Channel number
      int     m_nBins;      ///< Number of bins in the created histogram
      double  m_lowerBound; ///< Lower bound of the created histogram
      double  m_upperBound; ///< Upper bound of the created histogram
      QString m_name;       ///< Name of the channel
      int     m_threshold;  ///< Lower threshold applied for the readout

      mutable msg::Logger m_logger; ///< Logger member

   }; // class ChannelConfig

} // namespace v775

#endif // CDA_DEVICES_V775_CHANNELCONFIG_H
