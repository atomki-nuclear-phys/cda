// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_GUI_SIMPLE_DAQ_CAMACREADERRUNNER_H
#define CDA_GUI_SIMPLE_DAQ_CAMACREADERRUNNER_H

// Qt include(s):
#include <QtCore/QString>
#include <QtGui/QWidget>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/msg/Logger.h"
#   include "cdadaq/common/AppRunner.h"
#else
#   include "msg/Logger.h"
#   include "common/AppRunner.h"
#endif

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
    *  @short Widget for starting cda-camac-reader on the local machine
    *
    *         This Qt widget makes it easy to run the cda-camac-reader
    *         application on the local machine. It should in principle
    *         be used in conjunction with the other *Runner classes in
    *         this directory.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class CamacReaderRunner : public QWidget {

      Q_OBJECT

   public:
      /// Standard QWidget-type constructor
      CamacReaderRunner( QWidget* parent = 0, Qt::WindowFlags flags = 0 );
      /// Destructor
      ~CamacReaderRunner();

      /// Set the name of the configuration file
      void setConfigFileName( const QString& fileName );
      /// Get the name of the configuration file
      const QString& getConfigFileName() const;

      /// Set the address of the message server(s)
      void setMsgServerAddress( const QString& address );
      /// Get the address of the message server(s)
      const QString& getMsgServerAddress() const;

      /// Set the address of the statistics server(s)
      void setStatServerAddress( const QString& address );
      /// Get the address of the statistics server(s)
      const QString& getStatServerAddress() const;

      /// Set the address where cda-hbook-writer listenes for events
      void setHBookWriterAddress( const QString& address );
      /// Get the address where cda-hbook-writer listenes for events
      const QString& getHBookWriterAddress() const;

      /// Set the address where cda-glomem-writer listened for events
      void setGlomemWriterAddress( const QString& address );
      /// Get the address where cda-glomem-writer listened for events
      const QString& getGlomemWriterAddress() const;

      /// Set the verbosity level of the cda-camac-reader application
      void setVerbosity( msg::Level verbosity );
      /// Get the verbosity level of the cda-camac-reader application
      msg::Level getVerbosity() const;

   public slots:
      /// Set whether cda-hbook-writer is running already
      void setHBookWriterRunning( bool running );
      /// Set whether cda-glomem-writer is running already
      void setGlomemWriterRunning( bool running );

   private slots:
      /// Internal function starting and stopping cda-camac-reader
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
      QString m_statServerAddress;   ///< Address of the statistics server
      QString m_hbookWriterAddress;  ///< Address of cda-hbook-writer
      QString m_glomemWriterAddress; ///< Address of cda-glomem-writer
      msg::Level m_level;            ///< Output level of cda-camac-reader

      bool m_hbookWriterUpdating; ///< Flag showing if cda-hbook-writer is in the file name update process
      bool m_hbookWriterRunning;  ///< Flag showing if cda-hbook-writer is running
      bool m_glomemWriterRunning; ///< Flag showing if cda-glomem-writer is running

      mutable daq::AppRunner m_runner; ///< The object starting cda-camac-reader
      mutable msg::Logger m_logger;    ///< Internal logger object

   }; // class CamacReaderRunner

} // namespace simple_daq

#endif // CDA_GUI_SIMPLE_DAQ_CAMACREADERRUNNER_H
