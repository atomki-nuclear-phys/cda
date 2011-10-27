// $Id$

// Local include(s):
#include "Fragment.h"

namespace ev {

   Fragment::Fragment()
      : m_moduleNumber( -1 ), m_dataWords() {

   }

   int Fragment::getModuleID() const {

      return m_moduleNumber;
   }

   const std::vector< uint32_t >& Fragment::getDataWords() const {

      return m_dataWords;
   }

   void Fragment::setModuleID( int value ) {

      m_moduleNumber = value;
      return;
   }

   void Fragment::addDataWord( uint32_t dataWord ) {

      m_dataWords.push_back( dataWord );
      return;
   }

   void Fragment::clear() {

      m_moduleNumber = -1;
      m_dataWords.clear();

      return;
   }

} // namespace ev
