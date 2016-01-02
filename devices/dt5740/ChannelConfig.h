// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_DT5740_CHANNELCONFIG_H
#define CDA_DEVICES_DT5740_CHANNELCONFIG_H

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
   class ChannelConfig : virtual public dev::IConfig {

      /// Declare the Device class a friend of this class
      friend class dt5740::Device;

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( dt5740::ChannelConfig )

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

      /// Get the number of channels for the time monitoring histogram
      int            getTimeNumberOfChannels() const;
      /// Get the lower bound of the time monitoring histogram
      double         getTimeLowerBound() const;
      /// Get the upper bound of the time monitoring histogram
      double         getTimeUpperBound() const;
      /// Get the name of the time channel
      const QString& getTimeName() const;

      /// Set the number of channels for the time monitoring histogram
      void setTimeNumberOfChannels( int value );
      /// Set the lower bound of the time monitoring histogram
      void setTimeLowerBound( double value );
      /// Set the upper bound of the time monitoring histogram
      void setTimeUpperBound( double value );
      /// Set the name of the time channel
      void setTimeName( const QString& value );

      /// Get the number of channels for the energy monitoring histogram
      int            getEnergyNumberOfChannels() const;
      /// Get the lower bound of the energy monitoring histogram
      double         getEnergyLowerBound() const;
      /// Get the upper bound of the energy monitoring histogram
      double         getEnergyUpperBound() const;
      /// Get the name of the energy channel
      const QString& getEnergyName() const;

      /// Set the number of channels for the energy monitoring histogram
      void setEnergyNumberOfChannels( int value );
      /// Set the lower bound of the energy monitoring histogram
      void setEnergyLowerBound( double value );
      /// Set the upper bound of the energy monitoring histogram
      void setEnergyUpperBound( double value );
      /// Set the name of the energy channel
      void setEnergyName( const QString& value );

      /// Get the channel's number
      int  getChannelNumber() const;
      /// Set the channel's number
      void setChannelNumber( int number );

      /// Get the name of the raw ntuple variable
      const QString& getRawName() const;
      /// Set the name of the raw ntuple variable
      void setRawName( const QString& value );

      /// Print the configuration of the channel
      void printConfig( msg::Level level = msg::INFO ) const;

   private:
      /// Clear the channel's configuration
      void clear();

      QString m_rawName; ///< Name of the raw variable possibly written to the ntuple

      int     m_timeNumberOfChannels; ///< Number of channels in the created histogram
      double  m_timeLowerBound;       ///< Lower bound of the created histogram
      double  m_timeUpperBound;       ///< Upper bound of the created histogram
      QString m_timeName;             ///< Name of the channel

      int     m_energyNumberOfChannels; ///< Number of channels in the created histogram
      double  m_energyLowerBound;       ///< Lower bound of the created histogram
      double  m_energyUpperBound;       ///< Upper bound of the created histogram
      QString m_energyName;             ///< Name of the channel

      int     m_channelNumber;    ///< The channel's number in the device

      mutable msg::Logger m_logger; ///< Logger member

   }; // class ChannelConfig

} // namespace dt5740

#endif // CDA_DEVICES_DT5740_CHANNELCONFIG_H
