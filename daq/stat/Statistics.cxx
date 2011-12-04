// $Id$

// Local include(s):
#include "Statistics.h"

namespace cdastat {

   Statistics::Statistics( quint32 processedEvents, const QString& source,
                           const QTime& statTime )
      : m_processedEvents( processedEvents ), m_source( source ),
        m_statTime( statTime ) {

   }

   /**
    * @param events The number of events processed by the DAQ system
    */
   void Statistics::setProcessedEvents( quint32 events ) {

      m_processedEvents = events;
      return;

   }

   /**
    * @returns The number of events processed by the DAQ system
    */
   quint32 Statistics::getProcessedEvents() const {

      return m_processedEvents;

   }

   /**
    * @param source The source of the statistics
    */
   void Statistics::setSource( const QString& source ) {

      m_source = source;
      return;

   }

   /**
    * @returns The source of the statistics
    */
   const QString& Statistics::getSource() const {

      return m_source;

   }

   /**
    * @param statTime The time when this object was created
    */
   void Statistics::setStatTime( const QTime& statTime ) {

      m_statTime = statTime;
      return;

   }

   /**
    * @returns The time when this object was created
    */
   const QTime& Statistics::getStatTime() const {

      return m_statTime;

   }

   bool Statistics::operator== ( const Statistics& stat ) const {

      return ( ( m_processedEvents == stat.m_processedEvents ) &&
               ( m_source == stat.m_source ) &&
               ( m_statTime == stat.m_statTime ) );

   }

} // namespace cdastat
