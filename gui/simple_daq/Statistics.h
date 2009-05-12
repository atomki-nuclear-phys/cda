// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_GUI_SIMPLE_DAQ_STATISTICS_H
#define CDA_GUI_SIMPLE_DAQ_STATISTICS_H

// Qt include(s):
#include <QtCore/QString>
#include <QtGui/QWidget>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/common/Address.h"
#   include "cdadaq/stat/Statistics.h"
#   include "cdadaq/stat/Server.h"
#else
#   include "common/Address.h"
#   include "stat/Statistics.h"
#   include "stat/Server.h"
#endif

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QGroupBox )
QT_FORWARD_DECLARE_CLASS( QLabel )
QT_FORWARD_DECLARE_CLASS( QTimer )

namespace simple_daq {

   using QT_PREPEND_NAMESPACE( QGroupBox );
   using QT_PREPEND_NAMESPACE( QLabel );

   class Statistics : public QWidget {

      Q_OBJECT

   public:
      Statistics( const QString& iconPath, QWidget* parent = 0,
                  Qt::WindowFlags flags = 0 );
      ~Statistics();

      void setStatAddress( const Address& address );
      Address getStatAddress() const;

   private slots:
      void updateStat( const stat::Statistics& stat );
      void updateStat();

   private:
      QGroupBox* m_mainBox;

      QLabel* m_readEventsLabel;
      QLabel* m_readEvents;
      QLabel* m_eventRateLabel;
      QLabel* m_eventRate;

      QTimer* m_rateTimer;

      stat::Statistics m_lastStat;
      stat::Server m_server;

   }; // class Statistics

} // namespace simple_daq

#endif // CDA_GUI_SIMPLE_DAQ_STATISTICS_H
