// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_GUI_SIMPLE_DAQ_ROOTWRITERRUNNER_H
#define CDA_GUI_SIMPLE_DAQ_ROOTWRITERRUNNER_H

// STL include(s):
#include <set>

// Qt include(s):
#include <QtCore/QString>
#include <QWidget>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/msg/Logger.h"
#   include "cdadaq/common/AppRunner.h"
#else
#   include "msg/Logger.h"
#   include "common/AppRunner.h"
#endif

// Local include(s):
#include "../common/Export.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QGroupBox )
QT_FORWARD_DECLARE_CLASS( QPushButton )
QT_FORWARD_DECLARE_CLASS( QLabel )
QT_FORWARD_DECLARE_CLASS( QLineEdit )
QT_FORWARD_DECLARE_CLASS( QSpinBox )

namespace simple_daq {

   // Make sure that the Qt classes are available in this namespace:
   using QT_PREPEND_NAMESPACE( QGroupBox );
   using QT_PREPEND_NAMESPACE( QPushButton );
   using QT_PREPEND_NAMESPACE( QLabel );
   using QT_PREPEND_NAMESPACE( QLineEdit );
   using QT_PREPEND_NAMESPACE( QSpinBox );

   /**
    *  @short Widget for starting cda-root-writer on the local machine
    *
    *         This Qt widget makes it easy to run the cda-root-writer
    *         application on the local machine. It should in principle
    *         be used in conjunction with the other *Runner classes in
    *         this directory.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class CDAGUI_EXPORT RootWriterRunner : public QWidget {

      Q_OBJECT

   public:
      /// Standard QWidget-type constructor
      RootWriterRunner( QWidget* parent = 0, Qt::WindowFlags flags = 0 );
      /// Destructor
      ~RootWriterRunner();

      /// Set the name of the configuration file
      void setConfigFileName( const QString& fileName );
      /// Get the name of the configuration file
      const QString& getConfigFileName() const;

      /// Set the address of the message server(s)
      void setMsgServerAddress( const QString& address );
      /// Get the address of the message server(s)
      const QString& getMsgServerAddress() const;

      /// Set the address where cda-root-writer listenes for events
      void setEventAddress( const QString& address );
      /// Get the address where cda-root-writer listenes for events
      const QString& getEventAddress() const;

      /// Set the verbosity level of the cda-root-writer application
      void setVerbosity( msg::Level verbosity );
      /// Get the verbosity level of the cda-root-writer application
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
      /// Internal function starting and stopping cda-root-writer
      void startApp( bool start );

   private:
      QGroupBox*   m_mainBox;       ///< Main box holding graphcal objects
      QPushButton* m_starterButton; ///< Button starting the application
      QLabel*      m_processStatus; ///< Label giving feedback of the status
      QLabel*      m_updateFrequencyLabel; ///< Description of file name update frequency
      QSpinBox*    m_updateFrequency; ///< File name update frequency chooser
      QLabel*      m_fileNameLabel; ///< Label telling what the line edit is for
      QLineEdit*   m_fileNameEdit;  ///< Input field for the output file name

      QString m_configFileName;   ///< Name of the configuration file
      QString m_msgServerAddress; ///< Address of the message server(s)
      QString m_eventAddress;     ///< Address of cda-root-writer
      /// Address(es) of the statistics server(s)
      std::set< QString > m_statServerAddresses;
      msg::Level m_level;         ///< Output level of cda-root-writer

      mutable daq::AppRunner m_runner; ///< The object starting cda-root-writer
      mutable msg::Logger m_logger;    ///< Internal logger object

   }; // class RootWriterRunner

} // namespace simple_daq

#endif // CDA_GUI_SIMPLE_DAQ_ROOTWRITERRUNNER_H
