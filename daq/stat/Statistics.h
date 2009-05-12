// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DAQ_STAT_STATISTICS_H
#define CDA_DAQ_STAT_STATISTICS_H

// Qt include(s):
#include <QtCore/QTime>

namespace stat {

   class Statistics {

   public:
      Statistics( quint32 processedEvents = 0,
                  const QTime& statTime = QTime::currentTime() );

      void setProcessedEvents( quint32 events );
      quint32 getProcessedEvents() const;

      void setStatTime( const QTime& statTime );
      const QTime& getStatTime() const;

   private:
      quint32 m_processedEvents;
      QTime m_statTime;

   }; // class Statistics

} // namespace stat

#endif // CDA_DAQ_STAT_STATISTICS_H
