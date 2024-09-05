// Dear emacs, this is -*- c++ -*-
#ifndef CDA_GUI_SIMPLE_DAQ_STATISTICS_H
#define CDA_GUI_SIMPLE_DAQ_STATISTICS_H

// Qt include(s):
#include <QWidget>

// CDA include(s):
#include "common/Address.h"
#include "stat/Statistics.h"
#include "stat/Server.h"

// Local include(s):
#include "../common/Export.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QGroupBox )
QT_FORWARD_DECLARE_CLASS( QLabel )
QT_FORWARD_DECLARE_CLASS( QTimer )

namespace simple_daq {

   // Use the Qt classes in this namespace:
   using QT_PREPEND_NAMESPACE( QGroupBox );
   using QT_PREPEND_NAMESPACE( QLabel );
   using QT_PREPEND_NAMESPACE( QTimer );

   /**
    *  @short Simple widget showing DAQ statistics
    *
    *         This simple widget shows some statistics information while the
    *         DAQ system is running. Very basic stuff...
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    */
   class CDAGUI_EXPORT Statistics : public QWidget {

      Q_OBJECT

   public:
      /// Standard Qt widget constructor
      Statistics( QWidget* parent = 0,
                  Qt::WindowFlags flags = Qt::WindowFlags() );
      /// Destructor
      ~Statistics();

      /// Set the address on which to listen for incoming statistics
      void setStatAddress( const Address& address );
      /// Get the address on which the widget is listening for incoming statistics
      Address getStatAddress() const;

   private slots:
      /// Slot updating the statistics using the object given to it
      void updateStat( const cdastat::Statistics& stat );
      /// Slot updating the statistics when no statistics has been received
      void updateStat();

   private:
      QGroupBox* m_mainBox; ///< Box holding all other widgets

      QLabel* m_readEventsLabel; ///< "Events processed" label
      QLabel* m_readEvents; ///< Label showing the number of read events
      QLabel* m_eventRateLabel; ///< "Event rate" label
      QLabel* m_eventRate; ///< Label showing the current event rate

      QTimer* m_rateTimer; ///< Timer resetting the statistics if needed

      cdastat::Statistics m_lastStat; ///< The last statistics that was received
      cdastat::Server m_server;       ///< Statistics server

   }; // class Statistics

} // namespace simple_daq

#endif // CDA_GUI_SIMPLE_DAQ_STATISTICS_H
