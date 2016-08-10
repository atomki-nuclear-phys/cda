// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_DT5740_GROUPCONFIG_H
#define CDA_DEVICES_DT5740_GROUPCONFIG_H

// System include(s):
#include <memory>

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#include "device/IConfig.h"
#include "msg/Logger.h"

// Local include(s):
#include "ChannelConfig.h"

namespace dt5740 {

   /**
    *  @short Configuration for a single channel group
    *
    *         The input channels of the DT5740 are divided into
    *         4 groups, with 8 channels each. Much of the configuration
    *         for the channels can be set on the group level. This
    *         class stores the configuration options of one of these
    *         groups.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class GroupConfig : virtual public dev::IConfig {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( dt5740::GroupConfig )

   public:
      /// Default constructor
      GroupConfig();
      /// Copy constructor
      GroupConfig( const GroupConfig& parent );

      /// Assignment operator
      GroupConfig& operator= ( const GroupConfig& rh );

      /// Number of channels belonging to a channel group
      static const int CHANNELS_IN_GROUP = 8;

      /// Function reading the configuration in binary format
      virtual StatusCode readConfig( QIODevice& dev );
      /// Function writing the configuration in binary format
      virtual StatusCode writeConfig( QIODevice& dev ) const;

      /// Function reading the configuration in XML format
      virtual StatusCode readConfig( const QDomElement& node );
      /// Function writing the configuration in XML format
      virtual StatusCode writeConfig( QDomElement& node ) const;

      /// Get the number of this group
      int getGroupNumber() const;
      /// Set the number of this group
      void setGroupNumber( int value );

      /// Get if self-triggering is enabled
      bool getTrigEnabled() const;
      /// Set if self-triggering is enabled
      void setTrigEnabled( bool on );

      /// Get if trigger output to the front pane is enabled
      bool getTrigOutEnabled() const;
      /// Set if trigger output to the front pane is enabled
      void setTrigOutEnabled( bool on );

      /// Get the trigger threshold
      int getTrigThreshold() const;
      /// Set the trigger threshold
      void setTrigThreshold( int value );

      /// Get the trigger mask for the channels
      unsigned int getTrigMask() const;
      /// Set the trigger mask for the channels
      void setTrigMask( unsigned int mask );

      /// Get the DC offset for the group
      int getDCOffset() const;
      /// Set the DC offset for the group
      void setDCOffset( int value );

      /// Enable/disable one of the channels
      void enableChannel( int channel, bool enable );
      /// Access one of the channels
      ChannelConfig* getChannel( int channel );
      /// Access one of the channels (constant version)
      const ChannelConfig* getChannel( int channel ) const;

      /// Clear the group's configuration
      void clear();

   private:
      int m_groupNumber; ///< Group number (0..3)

      int          m_trigThreshold; ///< Trigger threshold for the group
      int          m_dcOffset; ///< DC offset for the group
      unsigned int m_trigMask; ///< Mask for enabling channels to trigger
      bool         m_trigEnabled; ///< Enable triggering on this group
      bool         m_trigOutEnabled; ///< Enable to send triggers to front panel

      /// Configuration of the input channels belonging to this group
      std::unique_ptr< ChannelConfig > m_channels[ CHANNELS_IN_GROUP ];

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class GroupConfig

} // namespace dt5740

#endif // CDA_DEVICES_DT5740_GROUPCONFIG_H
