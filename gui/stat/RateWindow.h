// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_GUI_STAT_RATEWINDOW_H
#define CDA_GUI_STAT_RATEWINDOW_H

// Qt include(s):
#include <QWidget>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdadaq/stat/Statistics.h"
#else
#   include "stat/Statistics.h"
#endif

// Local include(s):
#include "../common/Export.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QLabel )
QT_FORWARD_DECLARE_CLASS( QTimer )
QT_FORWARD_DECLARE_CLASS( QString )

namespace cdastat {

   // Forward declaration(s):
   class RateWidget;

   // Bring the Qt objects into this namespace:
   using QT_PREPEND_NAMESPACE( QLabel );
   using QT_PREPEND_NAMESPACE( QTimer );
   using QT_PREPEND_NAMESPACE( QString );

   /**
    *  @short Sub-window used by stat::MDIView to display the status of one process
    *
    *         This window can present the event processing status of one process
    *         sending its statistics information to the application. It doesn't do
    *         much on its own, but it can interact nicely with stat::MDIView...
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class CDAGUI_EXPORT RateWindow : public QWidget {

      Q_OBJECT

   public:
      /// Standard QWidget-style constructor
      RateWindow( QWidget* parent = 0, Qt::WindowFlags flags = 0 );
      /// Destructor
      ~RateWindow();

      /// Get the identifier of the source of the statistics
      const QString& getSource() const;

   signals:
      /// Signal sent periodically with new values of the calculated rate
      /**
       * @param source The identifyer of the source of the calulated rate
       * @param rate The calculated event processing rate
       */
      void newRateAvailable( const QString& source, qreal rate );
      /// Signal sent just before the window is closed
      /**
       * @param source The identifyer of the source of the calulated rate
       */
      void aboutToClose( const QString& source );

   protected:
      /// Re-implemented function from QWidget
      virtual void closeEvent( QCloseEvent* event );

   public slots:
      /// Function updating the window with the new statistics information
      void updateStatistics( const cdastat::Statistics& stat );
      /// Function resetting the displayed statistics
      void resetStatistics();

   private slots:
      /// Slot used internally to update the displayed rate periodically
      void updateRate();

   private:
      /// Time interval to wait between two rate updates
      /**
       * The event processing rate is reported by this widget periodically.
       * This constant sets the period of this process.
       */
      static const quint32 RATE_UPDATE_TIMEOUT = 1500;
      /// Time after which the event rate should be reset to 0.0
      /**
       * The rate calculation depends on receiving new statistics objects
       * periodically. If a new object is not received for this length of
       * time (in miliseconds), then the rate is reset to 0.0.
       */
      static const quint32 RATE_RESET_TIMEOUT = 5000;

      RateWidget* m_rateWidget; ///< Main display of the calculated rate

      QLabel* m_sourceNameLabel; ///< Description of the statistics source name
      QLabel* m_sourceName; ///< Source of the displayed statistics
      QLabel* m_processedEventsLabel; ///< Description of the number of processed events
      QLabel* m_processedEvents; ///< Number of processed events
      QLabel* m_eventRateLabel; ///< Description of the event processing rate
      QLabel* m_eventRate; ///< Event processing rate

      QTimer* m_rateTimer; ///< Timer used to periodically update the displayed rate
      qreal m_currentRate; ///< Last calculated value of the rate
      Statistics m_lastStat; ///< Last statistics object received

      QTimer* m_resetTimer; ///< Timer used to reset the shown rate after a timeout

   }; // class RateWindow

} // namespace cdastat

#endif // CDA_GUI_STAT_RATEWINDOW_H
