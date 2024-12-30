//
// ATOMKI Common Data Acquisition
//
// (c) 2008-2024 ATOMKI, Debrecen, Hungary
//
// Apache License Version 2.0
//
#ifndef CDA_GUI_SIMPLE_DAQ_STATISTICS_H
#define CDA_GUI_SIMPLE_DAQ_STATISTICS_H

// Qt include(s).
#include <QWidget>

// System include(s).
#include <memory>

// Forward declaration(s).
class Address;
namespace cdastat {
class Statistics;
}

namespace simple_daq {

/**
 *  @short Simple widget showing DAQ statistics
 *
 *         This simple widget shows some statistics information while the
 *         DAQ system is running. Very basic stuff...
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */
class Statistics : public QWidget {

   Q_OBJECT

public:
   /// Standard Qt widget constructor
   Statistics(QWidget* parent = nullptr,
              Qt::WindowFlags flags = Qt::WindowFlags());
   /// Destructor
   ~Statistics();

   /// Set the address on which to listen for incoming statistics
   void setStatAddress(const Address& address);
   /// Get the address on which the widget is listening for incoming statistics
   Address getStatAddress() const;

private slots:
   /// Slot updating the statistics using the object given to it
   void updateStat(const cdastat::Statistics& stat);
   /// Slot updating the statistics when no statistics has been received
   void updateStat();

private:
   /// Internal data type
   struct Impl;
   /// Internal data
   std::unique_ptr<Impl> m_impl;

};  // class Statistics

}  // namespace simple_daq

#endif  // CDA_GUI_SIMPLE_DAQ_STATISTICS_H
