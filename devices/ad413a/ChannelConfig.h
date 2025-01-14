// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_AD413A_CHANNELCONFIG_H
#define CDA_DEVICES_AD413A_CHANNELCONFIG_H

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#include "device/IConfig.h"
#include "msg/Logger.h"

namespace ad413a {

/**
 *  @short Class describing one channel of a AD413A module
 *
 *         Each channel of a AD413A module has a number of properties.
 *         All these properties are stored in such objects.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 *
 * $Revision$
 * $Date$
 */
class ChannelConfig : virtual public dev::IConfig {

   // To get the tr() function:
   Q_DECLARE_TR_FUNCTIONS(ad413a::ChannelConfig)

public:
   /// Constructor
   ChannelConfig();

   /// Function reading the configuration in binary format
   virtual StatusCode readConfig(QIODevice& dev);
   /// Function writing the configuration in binary format
   virtual StatusCode writeConfig(QIODevice& dev) const;

   /// Function reading the configuration in XML format
   virtual StatusCode readConfig(const QDomElement& node);
   /// Function writing the configuration in XML format
   virtual StatusCode writeConfig(QDomElement& node) const;

   /// Get the subaddress of this channel
   int getSubaddress() const;
   /// Get the low level discriminant of this channel
   int getLowLevelDiscriminant() const;
   /// Get the number of channels for the monitoring histogram
   int getNumberOfChannels() const;
   /// Get the lower bound of the monitoring histogram
   double getLowerBound() const;
   /// Get the upper bound of the monitoring histogram
   double getUpperBound() const;
   /// Get the name of the channel
   const QString& getName() const;

   /// Set the subaddress of this channel
   void setSubaddress(int value);
   /// Set the low level discriminant of this channel
   void setLowLevelDiscriminant(int lld);
   /// Set the number of channels for the monitoring histogram
   void setNumberOfChannels(int value);
   /// Set the lower bound of the monitoring histogram
   void setLowerBound(double value);
   /// Set the upper bound of the monitoring histogram
   void setUpperBound(double value);
   /// Set the name of the channel
   void setName(const QString& value);

   /// Print the configuration of the channel
   void printConfig(msg::Level level = msg::INFO) const;

private:
   /// Clear the channel's configuration
   void clear();

   int m_subaddress;        ///< Subaddress of this channel
   int m_lld;               ///< Low Level Discriminant for the channel
   int m_numberOfChannels;  ///< Number of channels in the created histogram
   double m_lowerBound;     ///< Lower bound of the created histogram
   double m_upperBound;     ///< Upper bound of the created histogram
   QString m_name;          ///< Name of the channel

   mutable msg::Logger m_logger;  ///< Logger member

};  // class ChannelConfig

}  // namespace ad413a

#endif  // CDA_DEVICES_AD413A_CHANNELCONFIG_H
