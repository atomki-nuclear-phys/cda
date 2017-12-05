// Dear emacs, this is -*- c++ -*-
#ifndef CDA_DAQ_STAT_STATISTICS_H
#define CDA_DAQ_STAT_STATISTICS_H

// Qt include(s):
#include <QTime>
#include <QString>

// Local include(s):
#include "../common/Export.h"

/**
 *  @short Namespace for DAQ statistics classes
 *
 *         This namespace collects the classes that are involved in the
 *         distribution of DAQ statistics information.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */
namespace cdastat {

   /**
    *  @short Class holding information about the status of the DAQ system
    *
    *         This class is the one distributed by the statistics system
    *         to the applications listening to DAQ statistics. It's quite
    *         a simple class really, since the DAQ system itself is quite
    *         simple...
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    */
   class CDADAQ_EXPORT Statistics {

   public:
      /// Constructor with all the properties of the object
      Statistics( quint32 processedEvents = 0,
                  const QString& source = "cda-daq",
                  const QTime& statTime = QTime::currentTime(),
                  quint32 lostEvents = 0 );

      /// Set the number of events processed by the DAQ
      void setProcessedEvents( quint32 events );
      /// Get the number of events processed by the DAQ
      quint32 getProcessedEvents() const;

      /// Set the number of events lost by the DAQ system
      void setLostEvents( quint32 events );
      /// Get the number of events lost by the DAQ system
      quint32 getLostEvents() const;

      /// Set the source of the statistics
      void setSource( const QString& source );
      /// Get the source of the statistics
      const QString& getSource() const;

      /// Set the moment when this object was created
      void setStatTime( const QTime& statTime );
      /// Get the moment when this object was created
      const QTime& getStatTime() const;

      /// Operator checking the equality of two statistics objects
      bool operator== ( const Statistics& stat ) const;

   private:
      quint32 m_processedEvents; ///< Number of processed events
      quint32 m_lostEvents;      ///< Events lost due to DAQ issues
      QString m_source;          ///< Source of the statistics
      QTime m_statTime;          ///< Time of the making of this object

   }; // class Statistics

} // namespace cdastat

#endif // CDA_DAQ_STAT_STATISTICS_H
