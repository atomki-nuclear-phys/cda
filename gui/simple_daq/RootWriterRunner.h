//
// ATOMKI Common Data Acquisition
//
// (c) 2008-2024 ATOMKI, Debrecen, Hungary
//
// Apache License Version 2.0
//
#ifndef CDA_GUI_SIMPLE_DAQ_ROOTWRITERRUNNER_H
#define CDA_GUI_SIMPLE_DAQ_ROOTWRITERRUNNER_H

// CDA include(s).
#include "msg/Level.h"

// Qt include(s).
#include <QString>
#include <QWidget>

// System include(s).
#include <memory>

namespace simple_daq {

/**
 *  @short Widget for starting cda-root-writer on the local machine
 *
 *         This Qt widget makes it easy to run the cda-root-writer
 *         application on the local machine. It should in principle
 *         be used in conjunction with the other *Runner classes in
 *         this directory.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */
class RootWriterRunner : public QWidget {

   Q_OBJECT

public:
   /// Standard QWidget-type constructor
   RootWriterRunner(QWidget* parent = 0,
                    Qt::WindowFlags flags = Qt::WindowFlags());
   /// Destructor
   ~RootWriterRunner();

   /// Set the name of the configuration file
   void setConfigFileName(const QString& fileName);
   /// Get the name of the configuration file
   const QString& getConfigFileName() const;

   /// Set the address of the message server(s)
   void setMsgServerAddress(const QString& address);
   /// Get the address of the message server(s)
   const QString& getMsgServerAddress() const;

   /// Set the address where cda-root-writer listenes for events
   void setEventAddress(const QString& address);
   /// Get the address where cda-root-writer listenes for events
   const QString& getEventAddress() const;

   /// Set the verbosity level of the cda-root-writer application
   void setVerbosity(msg::Level verbosity);
   /// Get the verbosity level of the cda-root-writer application
   msg::Level getVerbosity() const;

   /// Overrides the QWidget::setEnabled(...) function
   void setEnabled(bool status);

signals:
   /// Signal showing if the application is running
   void running(bool);
   /// Signal sending the event receiver address of this application
   void receiverRunning(bool, const QString&);

public slots:
   /// Set the address of a statistics receiver
   void setStatServerAddress(bool status, const QString& address);

private slots:
   /// Internal function starting and stopping cda-root-writer
   void startApp(bool start);

private:
   /// Internal data type
   struct Impl;
   /// Internal data
   std::unique_ptr<Impl> m_impl;

};  // class RootWriterRunner

}  // namespace simple_daq

#endif  // CDA_GUI_SIMPLE_DAQ_ROOTWRITERRUNNER_H
