// Dear emacs, this is -*- c++ -*-
#ifndef CDA_GUI_SIMPLE_DAQ_CAENREADERRUNNER_H
#define CDA_GUI_SIMPLE_DAQ_CAENREADERRUNNER_H

// STL include(s):
#include <set>

// Qt include(s):
#include <QString>
#include <QWidget>

// CDA include(s):
#include "msg/Logger.h"
#include "common/AppRunner.h"

// Local include(s):
#include "../common/Export.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QGroupBox )
QT_FORWARD_DECLARE_CLASS( QPushButton )
QT_FORWARD_DECLARE_CLASS( QLabel )

namespace simple_daq {

   // Make sure that the Qt classes are available in this namespace:
   using QT_PREPEND_NAMESPACE( QGroupBox );
   using QT_PREPEND_NAMESPACE( QPushButton );
   using QT_PREPEND_NAMESPACE( QLabel );

   /**
    *  @short Widget for starting cda-caen-reader on the local machine
    *
    *         This Qt widget makes it easy to run the cda-caen-reader
    *         application on the local machine. It should in principle
    *         be used in conjunction with the other *Runner classes in
    *         this directory.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    */
   class CDAGUI_EXPORT CaenReaderRunner : public QWidget {

      Q_OBJECT

   public:
      /// Standard QWidget-type constructor
      CaenReaderRunner( QWidget* parent = 0,
                        Qt::WindowFlags flags = Qt::WindowFlags() );
      /// Destructor
      ~CaenReaderRunner();

      /// Set the name of the configuration file
      void setConfigFileName( const QString& fileName );
      /// Get the name of the configuration file
      const QString& getConfigFileName() const;

      /// Set the address of the message server(s)
      void setMsgServerAddress( const QString& address );
      /// Get the address of the message server(s)
      const QString& getMsgServerAddress() const;

      /// Set the verbosity level of the cda-caen-reader application
      void setVerbosity( msg::Level verbosity );
      /// Get the verbosity level of the cda-caen-reader application
      msg::Level getVerbosity() const;

      /// Overrides the QWidget::setEnabled(...) function
      void setEnabled( bool status );

   public slots:
      /// Set whether a writer application is running
      void setWriterRunning( bool running, const QString& address );
      /// Set the address of a statistics receiver
      void setStatServerAddress( bool status, const QString& address );

   private slots:
      /// Internal function starting and stopping cda-caen-reader
      void startApp( bool start );

   private:
      QGroupBox*   m_mainBox;        ///< Main box holding graphcal objects
      QPushButton* m_starterButton;  ///< Button starting the application
      QLabel*      m_processStatus;  ///< Label giving feedback of the status
      QLabel*      m_startTimeLabel; ///< "Start time" label
      QLabel*      m_startTime;      ///< Label with the starting time
      QLabel*      m_stopTimeLabel;  ///< "Stop time" label
      QLabel*      m_stopTime;       ///< Label with the stopping time

      QString m_configFileName;      ///< Name of the configuration file
      QString m_msgServerAddress;    ///< Address of the message server
      /// Address(es) of the statistics server(s)
      std::set< QString > m_statServerAddresses;
      /// Addresses of event listener applications
      std::set< QString > m_eventListenerAddresses;
      msg::Level m_level;            ///< Output level of cda-caen-reader

      mutable daq::AppRunner m_runner; ///< The object starting cda-caen-reader
      mutable msg::Logger m_logger;    ///< Internal logger object

   }; // class CaenReaderRunner

} // namespace simple_daq

#endif // CDA_GUI_SIMPLE_DAQ_CAENREADERRUNNER_H
