// $Id$

// Local include(s):
#include "Fragment.h"

namespace ev {

   Fragment::Fragment()
      : m_crateNumber( 0 ), m_moduleNumber( -1 ), m_dataWords() {

   }

   int Fragment::getCrateNumber() const {

      return m_crateNumber;
   }

   int Fragment::getModuleID() const {

      return m_moduleNumber;
   }

   const std::vector< uint32_t >& Fragment::getDataWords() const {

      return m_dataWords;
   }

   void Fragment::setCrateNumber( int value ) {

      m_crateNumber = value;
      return;
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

      m_crateNumber = 0;
      m_moduleNumber = -1;
      m_dataWords.clear();

      return;
   }

} // namespace ev
