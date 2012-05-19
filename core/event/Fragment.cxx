// $Id$

// Local include(s):
#include "Fragment.h"

namespace ev {

   /**
    * The constructor is able to set the module ID right away, making
    * it possible to write fewer lines of code when constructing
    * the event fragment in the readout classes.
    *
    * @param moduleID The ID of the device that the fragment came from
    */
   Fragment::Fragment( Fragment::Identifier_t moduleID )
      : m_moduleID( moduleID ), m_dataWords() {

   }

   /**
    * @returns The identifier of the module that produced the fragment
    */
   Fragment::Identifier_t Fragment::getModuleID() const {

      return m_moduleID;
   }

   /**
    * @returns The data payload of the fragment
    */
   const Fragment::Payload_t& Fragment::getDataWords() const {

      return m_dataWords;
   }

   /**
    * @param value Identifier for the module sending the data
    */
   void Fragment::setModuleID( Fragment::Identifier_t value ) {

      m_moduleID = value;
      return;
   }

   /**
    * @param dataWord One 32-bit data word for the fragment payload
    */
   void Fragment::addDataWord( Fragment::Payload_t::value_type dataWord ) {

      m_dataWords.push_back( dataWord );
      return;
   }

   /**
    * This function can be used to estimate the size of the data that
    * we need to send over the network/write to file to serialize this
    * object.
    *
    * @returns The estimated size of the serialized object
    */
   uint32_t Fragment::sizeInBytes() const {

      return static_cast< uint32_t >( m_dataWords.size() *
                                      sizeof( Payload_t::value_type ) +
                                      sizeof( Identifier_t ) );
   }

   /**
    * By default the module ID of the fragment is also reset by this function,
    * but it can also be used to easily clear out just the data words as
    * well.
    *
    * @param clearModuleID Flag deciding whether or not to reset the module ID
    */
   void Fragment::clear( bool clearModuleID ) {

      if( clearModuleID ) {
         m_moduleID = 0xffffffff;
      }
      m_dataWords.clear();

      return;
   }

} // namespace ev
