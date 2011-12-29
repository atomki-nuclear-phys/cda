// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_GUI_SIMPLE_DAQ_GLOMEMWRITERRUNNER_H
#define CDA_GUI_SIMPLE_DAQ_GLOMEMWRITERRUNNER_H

// STL include(s):
#include <set>

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
    *  @short Widget for starting cda-glomem-writer on the local machine
    *
    *         This Qt widget makes it easy to run the cda-glomem-writer
    *         application on the local machine. It should in principle
    *         be used in conjunction with the other *Runner classes in
    *         this directory.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class GlomemWriterRunner : public QWidget {

      Q_OBJECT

   public:
      /// Standard QWidget-type constructor
      GlomemWriterRunner( QWidget* parent = 0, Qt::WindowFlags flags = 0 );
      /// Destructor
      ~GlomemWriterRunner();

      /// Set the name of the configuration file
      void setConfigFileName( const QString& fileName );
      /// Get the name of the configuration file
      const QString& getConfigFileName() const;

      /// Set the address of the message server(s)
      void setMsgServerAddress( const QString& address );
      /// Get the address of the message server(s)
      const QString& getMsgServerAddress() const;

      /// Set the address where cda-glomem-writer listenes for events
      void setEventAddress( const QString& address );
      /// Get the address where cda-glomem-writer listenes for events
      const QString& getEventAddress() const;

      /// Set the verbosity level of the cda-glomem-writer application
      void setVerbosity( msg::Level verbosity );
      /// Get the verbosity level of the cda-glomem-writer application
      msg::Level getVerbosity() const;

      /// Overrides the QWidget::setEnabled(...) function
      void setEnabled( bool status );

   signals:
      /// Signal showing if the application is running
      void running( bool );
      /// Signal sending the event receiver address of this application
      void receiverRunning( bool, const QString& );

   public slots:
      /// Set the address of a statistics receiver
      void setStatServerAddress( bool status, const QString& address );

   private slots:
      /// Internal function starting and stopping cda-glomem-writer
      void startApp( bool start );

   private:
      QGroupBox*   m_mainBox;       ///< Main box holding graphcal objects
      QPushButton* m_starterButton; ///< Button starting the application
      QLabel*      m_processStatus; ///< Label giving feedback of the status

      QString m_configFileName;   ///< Name of the configuration file
      QString m_msgServerAddress; ///< Address of the message server(s)
      QString m_eventAddress;     ///< Address of cda-glomem-writer
      /// Address(es) of the statistics server(s)
      std::set< QString > m_statServerAddresses;
      msg::Level m_level;         ///< Output level of cda-glomem-writer

      mutable daq::AppRunner m_runner; ///< The object starting cda-glomem-writer
      mutable msg::Logger m_logger;    ///< Internal logger object

   }; // class GlomemWriterRunner

} // namespace simple_daq

#endif // CDA_GUI_SIMPLE_DAQ_GLOMEMWRITERRUNNER_H
