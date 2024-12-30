//
// ATOMKI Common Data Acquisition
//
// (c) 2008-2024 ATOMKI, Debrecen, Hungary
//
// Apache License Version 2.0
//

// Local include(s).
#include "Statistics.h"

// CDA include(s):
#include "common/Address.h"
#include "stat/Server.h"
#include "stat/Statistics.h"

// Qt include(s).
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QString>
#include <QTimer>
#include <QVBoxLayout>

namespace simple_daq {

struct Statistics::Impl {

   /// Constructor
   Impl(QWidget& parent, const QString& boxTitle, const QString& reLabelText,
        const QString& reText, const QString& erLabelText,
        const QString& erText)
       : m_mainBox{boxTitle, &parent},
         m_layout{&m_mainBox},
         m_readEventsLabel(reLabelText, &m_mainBox),
         m_readEvents(reText, &m_mainBox),
         m_eventRateLabel(erLabelText, &m_mainBox),
         m_eventRate(erText, &m_mainBox),
         m_server(&parent) {

      //
      // Lay out the widgets.
      //
      m_mainLayout.addWidget(&m_mainBox);
      parent.setLayout(&m_mainLayout);

      m_layout.addWidget(&m_readEventsLabel, 0, 0,
                         Qt::AlignRight | Qt::AlignVCenter);
      m_layout.addWidget(&m_readEvents, 0, 1, Qt::AlignLeft | Qt::AlignVCenter);
      m_layout.addWidget(&m_eventRateLabel, 1, 0,
                         Qt::AlignRight | Qt::AlignVCenter);
      m_layout.addWidget(&m_eventRate, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
   }

   /// Layout connecting the parent to the main box
   QVBoxLayout m_mainLayout;

   /// Box holding all other widgets
   QGroupBox m_mainBox;

   /// Layout for the main box
   QGridLayout m_layout;

   /// "Events processed" label
   QLabel m_readEventsLabel;
   /// Label showing the number of read events
   QLabel m_readEvents;
   /// "Event rate" label
   QLabel m_eventRateLabel;
   /// Label showing the current event rate
   QLabel m_eventRate;

   ///< Timer resetting the statistics if needed
   QTimer m_rateTimer;

   /// The last statistics that was received
   cdastat::Statistics m_lastStat;
   /// Statistics server
   cdastat::Server m_server;
};

Statistics::Statistics(QWidget* parent, Qt::WindowFlags flags)
    : QWidget(parent, flags),
      m_impl{new Impl{*this, tr("DAQ statistics"), tr("Events processed:"),
                      tr("n/a"), tr("Event rate:"), tr("n/a Hz")}} {

   //
   // Set up statistics updates.
   //
   connect(&(m_impl->m_server),
           SIGNAL(statAvailable(const cdastat::Statistics&)), this,
           SLOT(updateStat(const cdastat::Statistics&)));

   //
   // Set up the timer that resets the statistics when a new stat information
   // isn't received for a given amount of time.
   //
   m_impl->m_rateTimer.setSingleShot(true);
   connect(&(m_impl->m_rateTimer), SIGNAL(timeout()), this, SLOT(updateStat()));
}

Statistics::~Statistics() {}

/**
 * @param address Address where the widget should listen to DAQ statistics
 */
void Statistics::setStatAddress(const Address& address) {

   m_impl->m_server.listen(address);
}

/**
 * @returns The address on which the widget is listening to DAQ statistics
 */
Address Statistics::getStatAddress() const {

   return {m_impl->m_server.serverAddress().toString(),
           m_impl->m_server.serverPort()};
}

/**
 * This function calculates the event processing rate, and displays it alongside
 * the total number of events processed so far.
 *
 * @param stat The latest statistics object
 */
void Statistics::updateStat(const cdastat::Statistics& stat) {

   //
   // Show the number of processed events according to the last
   // cdastat::Statistics object received, and calculate the event
   // processing rate using the last statistics object:
   //
   m_impl->m_readEvents.setText(QString::number(stat.getProcessedEvents()));
   if (stat.getSource() == m_impl->m_lastStat.getSource()) {
      // If the statistics is comign from the same source, then calculate the
      // rate:
      m_impl->m_eventRate.setText(
          QString::number(
              static_cast<double>(stat.getProcessedEvents() -
                                  m_impl->m_lastStat.getProcessedEvents()) /
              m_impl->m_lastStat.getStatTime().msecsTo(stat.getStatTime()) *
              1000.0) +
          " Hz");
   } else {
      // If the statistics is from a new source, wait with calculating the rate.
      m_impl->m_eventRate.setText("n/a Hz");
   }

   // Remember this as the last statistics object.
   m_impl->m_lastStat = stat;

   // If we don't receive an update in 5 seconds, let's declare the rate zero.
   m_impl->m_rateTimer.start(5000);
}

/**
 * This function resets the event rate to "0 Hz" if no new statistics has been
 * received for a given amount of time.
 */
void Statistics::updateStat() {

   m_impl->m_eventRate.setText("0 Hz");
   m_impl->m_lastStat.setStatTime(QTime::currentTime());
}

}  // namespace simple_daq
