// $Id$

// Local include(s):
#include "Statistics.h"

namespace stat {

   Statistics::Statistics( quint32 processedEvents, const QTime& statTime )
      : m_processedEvents( processedEvents ), m_statTime( statTime ) {

   }

   void Statistics::setProcessedEvents( quint32 events ) {

      m_processedEvents = events;
      return;

   }

   quint32 Statistics::getProcessedEvents() const {

      return m_processedEvents;

   }

   void Statistics::setStatTime( const QTime& statTime ) {

      m_statTime = statTime;
      return;

   }

   const QTime& Statistics::getStatTime() const {

      return m_statTime;

   }

} // namespace stat
