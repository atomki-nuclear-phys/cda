// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_GUI_STAT_RATEWINDOW_H
#define CDA_GUI_STAT_RATEWINDOW_H

// Qt include(s):
#include <QtGui/QWidget>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdadaq/stat/Statistics.h"
#else
#   include "stat/Statistics.h"
#endif

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QLabel )
QT_FORWARD_DECLARE_CLASS( QTimer )

namespace stat {

   // Forward declaration(s):
   class RateWidget;

   using QT_PREPEND_NAMESPACE( QLabel );
   using QT_PREPEND_NAMESPACE( QTimer );

   class RateWindow : public QWidget {

      Q_OBJECT

   public:
      RateWindow( QWidget* parent = 0, Qt::WindowFlags flags = 0 );
      ~RateWindow();

   signals:
      void newRateAvailable( double rate );

   public slots:
      void updateStatistics( const stat::Statistics& stat );
      void resetStatistics();

   private slots:
      void updateRate();

   private:
      stat::RateWidget* m_rateWidget;

      QLabel* m_sourceNameLabel;
      QLabel* m_sourceName;
      QLabel* m_processedEventsLabel;
      QLabel* m_processedEvents;
      QLabel* m_eventRateLabel;
      QLabel* m_eventRate;

      QTimer* m_rateTimer;
      double m_currentRate;
      Statistics m_lastStat;

      QTimer* m_resetTimer;

   }; // class RateWindow

} // namespace stat

#endif // CDA_GUI_STAT_RATEWINDOW_H
