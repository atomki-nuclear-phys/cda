// $Id$

// System include(s):
#include <cmath>

// Qt include(s):
#include <QtCore/QIODevice>
#include <QtCore/QDataStream>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/event/Fragment.h"
#else
#   include "event/Fragment.h"
#endif

// Local include(s):
#include "Device.h"

namespace dt5740 {

   //
   // Make sure that the following Qt classes are available in the
   // current namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QIODevice );
   using QT_PREPEND_NAMESPACE( QDataStream );
   using QT_PREPEND_NAMESPACE( QDomNode );
   using QT_PREPEND_NAMESPACE( QDomElement );

   Device::Device()
      : m_logger( "dt5740::Device" ) {

      // Set the ID of each group:
      for( int i = 0; i < NUMBER_OF_GROUPS; ++i ) {
         m_groups[ i ].setGroupNumber( i );
      }
   }

   Device::~Device() {

      clear();
   }

   bool Device::readConfig( QIODevice* dev ) {

      REPORT_VERBOSE( tr( "Reading configuration from binary input" ) );

      clear();

      // Read the properties of this class:
      QDataStream input( dev );
      input.setVersion( QDataStream::Qt_4_0 );

      // Read the connection parameters of the device:
      int ctype;
      input >> ctype;
      m_connType = caen::Digitizer::convertConnType( ctype );
      input >> m_linkNumber;

      // Read in the configuration of the groups:
      for( int i = 0; i < NUMBER_OF_GROUPS; ++i ) {
         if( ! m_groups[ i ].readConfig( dev ) ) {
            REPORT_ERROR( tr( "The configuration of a group couldn't be "
                              "read!" ) );
            return false;
         }
         if( m_groups[ i ].getGroupNumber() != i ) {
            REPORT_ERROR( tr( "There was an error reading the configuration "
                              "of a group" ) );
            return false;
         }
      }

      return true;
   }

   bool Device::writeConfig( QIODevice* dev ) const {

      REPORT_VERBOSE( tr( "Writing configuration to binary output" ) );

      // Write the properties of this class:
      QDataStream output( dev );
      output.setVersion( QDataStream::Qt_4_0 );

      // Write out the connection parameters of the device:
      output << caen::Digitizer::convertConnType( m_connType );
      output << m_linkNumber;

      // Write out the group configurations:
      for( int i = 0; i < NUMBER_OF_GROUPS; ++i ) {
         if( ! m_groups[ i ].writeConfig( dev ) ) {
            REPORT_ERROR( tr( "A problem happened while writing out a "
                              "group's configuration" ) );
            return false;
         }
      }

      return true;
   }

   bool Device::readConfig( const QDomElement& element ) {

      REPORT_VERBOSE( tr( "Reading configuration from XML input" ) );

      clear();

      //
      // Read the device wide configuration:
      //
      bool ok;

      const int ctype = element.attribute( "ConnType", "0" ).toInt( &ok );
      if( ! ok ) {
         REPORT_ERROR( tr( "Couldn't read connection type" ) );
         return false;
      }
      m_connType = caen::Digitizer::convertConnType( ctype );

      m_linkNumber = element.attribute( "LinkNumber", "0" ).toInt( &ok );
      if( ! ok ) {
         REPORT_ERROR( tr( "Couldn't read link number" ) );
         return false;
      }

      //
      // Configure the groups:
      //
      for( int i = 0; i < element.childNodes().size(); ++i ) {

         // Only process "Group" type child-nodes:
         if( element.childNodes().at( i ).nodeName() != "Group" ) {
            continue;
         }

         GroupConfig group;
         if( ! group.readConfig( element.childNodes().at( i ).toElement() ) ) {
            REPORT_ERROR( tr( "The configuration of a group couldn't be "
                              "read" ) );
            return false;
         }
         if( ( group.getGroupNumber() >= 0 ) &&
             ( group.getGroupNumber() < NUMBER_OF_GROUPS ) ) {
            m_groups[ group.getGroupNumber() ] = group;
         } else {
            REPORT_ERROR( tr( "There was an error reading the configuration "
                              "of a group" ) );
            return false;
         }
      }

      return true;
   }

   bool Device::writeConfig( QDomElement& element ) const {

      REPORT_VERBOSE( tr( "Writing configuration to XML output" ) );

      //
      // Write the device wide configuration:
      //
      element.setAttribute( "ConnType",
                            caen::Digitizer::convertConnType( m_connType ) );
      element.setAttribute( "LinkNumber", m_linkNumber );

      //
      // Create a new node for the configuration of each group:
      //
      for( int i = 0; i < NUMBER_OF_GROUPS; ++i ) {

         QDomElement gr_element =
            element.ownerDocument().createElement( "Group" );
         if( ! m_groups[ i ].writeConfig( gr_element ) ) {
            REPORT_ERROR( tr( "A problem happened while writing out a "
                              "group's configuration" ) );
            return false;
         }
         element.appendChild( gr_element );
      }

      return true;
   }

   unsigned int Device::getID() const {

      return m_linkNumber;
   }

   void Device::setID( unsigned int id ) {

      m_linkNumber = id;
      return;
   }

   void Device::clear() {

      // Reset the simple properties:
      m_connType = caen::Digitizer::USB;
      m_linkNumber = 0;

      // Clear all the groups:
      for( int i = 0; i < NUMBER_OF_GROUPS; ++i ) {
         m_groups[ i ].clear();
      }

      return;
   }

   /**
    * This function is used by the derived classes to decode the content
    * of the data words read from the device's memory during data
    * taking.
    *
    * @param fragment The raw data words coming from the device
    * @returns The data in an easier-to-digest format
    */
   Device::Data_t Device::decode( const ev::Fragment& fragment ) const {

      // The result object:
      Data_t result;
      result.resize( NUMBER_OF_GROUPS * GroupConfig::CHANNELS_IN_GROUP );

      // The first bit of the next data word. Remember that the first
      // 4 words belong to the header of the event:
      int bit_number = 128;

      // According to the documentation the readout format is as follows:
      //  - First we get all the samples for the first channel of the first
      //    group.
      //  - Next we get all the samples of the second channel of the first
      //    group.
      //  - etc.

      // Loop over the groups:
      for( int group = 0; group < NUMBER_OF_GROUPS; ++group ) {
         // Loop over the channels of the group:
         for( int channel = 0; channel < GroupConfig::CHANNELS_IN_GROUP;
              ++channel ) {
            // Skip the inactive channels:
            if( ! m_groups[ group ].getChannel( channel ) ) continue;
            // Reserve the needed space in the inner vector:
            result[ ( group * GroupConfig::CHANNELS_IN_GROUP ) +
                    channel ].resize( m_groups[ group ].getSamples() );
            // Loop over all the samples:
            for( int sample = 0; sample < m_groups[ group ].getSamples();
                 ++sample ) {
               // The data of this channel in this sample:
               unsigned int ch_data = 0;
               // Fill the data bit-by-bit:
               for( int abit = bit_number, i = 0; abit < bit_number + BITS_PER_CHANNEL;
                    ++abit, ++i ) {
                  const int word = abit / 32;
                  const int bit  = abit % 32;
                  // A security check:
                  if( word >= static_cast< int >( fragment.getDataWords().size() ) ) {
                     REPORT_ERROR( tr( "The received data fragment is too small!" ) );
                     return result;
                  }
                  if( ( fragment.getDataWords()[ word ] >> bit ) & 0x1 ) {
                     ch_data |= ( 0x1 << i );
                  }
               } // end of loop over bits
               // Add this data word to the result:
               result[ ( group * GroupConfig::CHANNELS_IN_GROUP ) +
                       channel ].push_back( ch_data );
               // Increment the bit position:
               bit_number += BITS_PER_CHANNEL;
            } // end of loop over samples
         } // end of loop over channels in group
      } // end of loop over groups

      return result;
   }

} // namespace dt5740
