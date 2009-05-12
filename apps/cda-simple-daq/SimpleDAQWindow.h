// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_APPS_CDA_SIMPLE_DAQ_SIMPLEDAQWINDOW_H
#define CDA_APPS_CDA_SIMPLE_DAQ_SIMPLEDAQWINDOW_H

// Qt include(s):
#include <QtGui/QMainWindow>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/msg/Logger.h"
#else
#   include "msg/Logger.h"
#endif

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QWidget )
namespace msg {
   class Server;
   class TextView;
} // namespace msg
namespace simple_daq {
   class Statistics;
   class CamacReaderRunner;
   class GlomemWriterRunner;
   class HBookWriterRunner;
} // namespace simple_daq

class SimpleDAQWindow : public QMainWindow {

   Q_OBJECT

public:
   SimpleDAQWindow( const QString& confFileName,
                    msg::Level verbosity );
   ~SimpleDAQWindow();

private:
   QWidget* m_centralWidget;

   msg::Server*   m_msgServer;
   msg::TextView* m_msgView;

   simple_daq::Statistics*         m_statistics;
   simple_daq::CamacReaderRunner*  m_camacReader;
   simple_daq::GlomemWriterRunner* m_glomemWriter;
   simple_daq::HBookWriterRunner*  m_hbookWriter;

   mutable msg::Logger m_logger;

}; // class SimpleDAQWindow

#endif // CDA_APPS_CDA_SIMPLE_DAQ_SIMPLEDAQWINDOW_H
