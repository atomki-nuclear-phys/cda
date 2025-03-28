// $Id$
/**
 *   @file apps/cda-raw-writer/main.cxx
 *  @short Main file for the RAW writer application
 *
 *         This file stores the code that runs the cda-raw-writer application.
 *         This application is used to permanently store the data coming from
 *         the DAQ system in an application specific RAW format.
 *
 * @author Attila Krasznahorkay Jr. <Attila.Krasznahorkay@cern.ch>
 *
 * $Revision$
 * $Date$
 */

// System include(s):
#include <csignal>
#include <cstdlib>

// Signal handler include:
#ifdef _MSC_VER
#include <signal.h>
#else
#include <unistd.h>
#endif

// STL include(s):
#include <iostream>
#include <map>

// Qt include(s):
#include <QtCore/QCoreApplication>
#include <QtCore/QDateTime>
#include <QtCore/QString>

// CDA include(s):
#include "CDAVersion.h"
#include "common/Sleep.h"
#include "config/ConfReader.h"
#include "device/Loader.h"
#include "event/Event.h"
#include "event/EventServer.h"
#include "i18n/Loader.h"
#include "msg/Logger.h"
#include "msg/Sender.h"
#include "stat/Sender.h"
#include "tclap/CmdLine.h"

// Local include(s):
#include "../common/plugins.h"
#include "../common/readConfig.h"
#include "Crate.h"
#include "FileWriter.h"

// Function forward declaration(s):
void shutDown(int);

// Global variable(s):
static msg::Logger g_logger("cda-raw-writer");
static raw::Crate* g_crate = 0;
static raw::FileWriter* g_fwriter = 0;
static quint32 g_evcount = 0;

/// Description printed to the console
static const char* description =
    "Program writing events which it receives, to a RAW format file. "
    "The output file contains the events in the binary format that is "
    "used for sending the data between the applications.\n\n"
    "This executable should normally be started by CDA internally. "
    "You should only start it by hand for debugging purposes.";

int main(int argc, char* argv[]) {

   //
   // Read the command line options:
   //
   TCLAP::CmdLine cmd(description, ' ', CDA_VERSION_STRING);
   TCLAP::ValueArg<int> verbosity("v", "verbosity", "Level of output verbosity",
                                  false, 3, "code");
   cmd.add(verbosity);
   TCLAP::MultiArg<std::string> msgservers("m", "msgservers",
                                           "Addresses of message servers",
                                           false, "address list");
   cmd.add(msgservers);
   TCLAP::ValueArg<std::string> config("c", "config",
                                       "Name of an XML config file, or "
                                       "address of a config server",
                                       true, "", "filename/address");
   cmd.add(config);
   TCLAP::ValueArg<std::string> evaddress("e", "evaddress",
                                          "Address on which to receive events",
                                          true, "", "address");
   cmd.add(evaddress);
   TCLAP::MultiArg<std::string> statistics(
       "s", "statistics", "Addresses of statistics reader clients", false,
       "address list");
   cmd.add(statistics);
   TCLAP::ValueArg<std::string> output(
       "o", "output", "Name of the output HBOOK file", true, "", "filename");
   cmd.add(output);
   TCLAP::ValueArg<int> updatefreq("u", "update-frequency",
                                   "Frequency of file name updates", false, 60,
                                   "minutes");
   cmd.parse(argc, argv);

   //
   // Set the destination of the messages:
   //
   for (const std::string& address : msgservers.getValue()) {
      msg::Sender::addAddress(Address(address.c_str()));
   }

   //
   // Load all the available translations:
   //
   QCoreApplication app(argc, argv);
   i18n::Loader trans_loader;
   if (!trans_loader.loadTranslations()) {
      g_logger << msg::FATAL
               << qApp->translate("cda-raw-writer",
                                  "Couldn't load the translations!")
               << msg::endmsg;
      return 1;
   }

   //
   // Translate the verbosity option:
   //
   std::map<int, msg::Level> v_map;
   v_map[1] = msg::VERBOSE;
   v_map[2] = msg::DEBUG;
   v_map[3] = msg::INFO;
   v_map[4] = msg::WARNING;
   v_map[5] = msg::ERROR;
   v_map[6] = msg::FATAL;
   v_map[7] = msg::ALWAYS;
   auto itr = v_map.find(verbosity.getValue());
   if (itr != v_map.end()) {
      msg::Sender::instance()->setMinLevel(itr->second);
   } else {
      g_logger << msg::FATAL
               << qApp->translate("cda-raw-writer",
                                  "Didn't recognise verbosity level setting\n"
                                  "Terminating...")
               << msg::endmsg;
      return 1;
   }

   //
   // Try to load all available device plugins:
   //
   if (dev::Loader::instance()->loadAll()) {
      g_logger << msg::INFO
               << qApp->translate("cda-raw-writer",
                                  "Successfully loaded all available devices")
               << msg::endmsg;
   } else {
      g_logger << msg::FATAL
               << qApp->translate("cda-raw-writer",
                                  "There was an error loading the devices")
               << msg::endmsg;
   }

   //
   // Create the crate object:
   //
   g_crate = new raw::Crate();
   g_crate->setLoader(dev::Loader::instance());

   //
   // Read the configuration.
   //
   if (apps::readConfig(config.getValue().c_str(), *g_crate).isFailure()) {
      g_logger << msg::FATAL
               << qApp->translate("cda-raw-writer",
                                  "Couldn't read the configuration!")
               << msg::endmsg;
      return 1;
   }

   //
   // Start an EventServer listening on the specified port:
   //
   ev::EventServer evserver;
   evserver.listen(Address(evaddress.getValue().c_str()));

   //
   // Open connections to all the statistics recepients. (Ignore connection
   // errors here, since statistics publishing is not a major concern...)
   //
   cdastat::Sender stat_sender;
   for (const std::string& server : statistics.getValue()) {
      stat_sender.addReceiver(Address(server.c_str()));
   }

   //
   // Connect the interrupt signal to the shutDown function:
   //
   signal(SIGINT, shutDown);
   signal(SIGTERM, shutDown);

   //
   // Construct the source string of the statistics objects that are sent out
   // during event processing:
   //
   QString statSource = "cda-raw-writer:";
   statSource += config.getValue().c_str();
   statSource += ":";
   statSource += QString::number(QCoreApplication::applicationPid());

   // Initialise the statistics information to something meaningful, then start
   // the statistics sender object:
   stat_sender.update(cdastat::Statistics(0, statSource));
   stat_sender.start();

   //
   // Let the user know what we're doing:
   //
   g_logger << msg::INFO
            << qApp->translate("cda-raw-writer", "RAW writing running...")
            << msg::endmsg;

   //
   // Create the thread that will take care of writing the output file(s):
   //
   g_fwriter = new raw::FileWriter(evserver, *g_crate);

   //
   // Decide if file name updating will be required:
   //
   QString fileName(output.getValue().c_str());
   if (fileName.contains("%1")) {
      //
      // Initialise writing to the first RAW file:
      //
      int filecounter = 1;
      if (!g_crate->initialize(
              fileName.arg(filecounter, (int)3, (int)10, QLatin1Char('0')))) {
         g_logger << msg::FATAL
                  << qApp->translate("cda-raw-writer",
                                     "Failed to initialise RAW file for data "
                                     "acquisition")
                  << msg::endmsg;
         return 1;
      } else {
         g_logger << msg::DEBUG
                  << qApp->translate("cda-raw-writer",
                                     "Initialised RAW file for data "
                                     "acquisition")
                  << msg::endmsg;
      }
      //
      // Start the file writing and monitoring (this) threads:
      //
      g_fwriter->start();
      QDateTime startTime(QDateTime::currentDateTime());
      for (;;) {

         // Check if the file writing thread is still running:
         if (!g_fwriter->isRunning()) {
            g_logger << msg::FATAL
                     << qApp->translate("cda-raw-writer",
                                        "The file writing thread "
                                        "unexpectedly died")
                     << msg::endmsg;
            shutDown(0);
         }

         // Update the statistics receivers:
         stat_sender.update(cdastat::Statistics(
             g_evcount + g_fwriter->processedEvents(), statSource));

         // If it's time to open a new file, let's do it:
         if (startTime.secsTo(QDateTime::currentDateTime()) >
             (60 * updatefreq.getValue())) {
            // Remember the current time:
            startTime = QDateTime::currentDateTime();
            // First off, let's stop the file writing thread:
            g_fwriter->stopProcessing();
            // Store how much data we saved to this file:
            g_evcount += g_fwriter->processedEvents();
            // Now close the current output file:
            g_crate->finalize();
            // Open a new file:
            ++filecounter;
            if (!g_crate->initialize(fileName.arg(filecounter, (int)3, (int)10,
                                                  QLatin1Char('0')))) {
               g_logger << msg::FATAL
                        << qApp->translate("cda-raw-writer",
                                           "Failed to initialise RAW file "
                                           "for data acquisition")
                        << msg::endmsg;
               return 1;
            } else {
               g_logger << msg::DEBUG
                        << qApp->translate("cda-raw-writer",
                                           "Initialised RAW file for data "
                                           "acquisition")
                        << msg::endmsg;
            }
            // Start the file writing thread once more:
            g_fwriter->start();
         }

         // Sleep for 2 seconds:
         common::Sleep(2000);
      }
   } else {
      //
      // Initialise writing to the RAW file:
      //
      if (!g_crate->initialize(fileName)) {
         g_logger << msg::FATAL
                  << qApp->translate("cda-raw-writer",
                                     "Failed to initialise RAW file for data "
                                     "acquisition")
                  << msg::endmsg;
         return 1;
      } else {
         g_logger << msg::DEBUG
                  << qApp->translate("cda-raw-writer",
                                     "Initialised RAW file for data "
                                     "acquisition")
                  << msg::endmsg;
      }
      //
      // Start the file writing and monitoring (this) threads:
      //
      g_fwriter->start();
      for (;;) {

         // Check if the file writing thread is still running:
         if (!g_fwriter->isRunning()) {
            g_logger << msg::FATAL
                     << qApp->translate("cda-raw-writer",
                                        "The file writing thread unexpectedly "
                                        "died")
                     << msg::endmsg;
            shutDown(0);
         }

         // Update the statistics receivers:
         stat_sender.update(
             cdastat::Statistics(g_fwriter->processedEvents(), statSource));

         // Sleep for 2 seconds:
         common::Sleep(2000);
      }
   }

   //
   // The code is never supposed to reach this statement, but it's nicer
   // to have the compiler satisfied...
   //
   return 0;
}

/**
 * This function makes sure that the output RAW file is properly closed
 * when the application terminates.
 */
void shutDown(int) {

   // Stop the event processing thread:
   g_fwriter->stopProcessing();

   // Close the current output file:
   g_crate->finalize();

   g_logger << msg::INFO
            << qApp->translate("cda-raw-writer",
                               "Total number of events processed: %1")
                   .arg(g_evcount + g_fwriter->processedEvents())
            << msg::endmsg;
   g_logger << msg::INFO
            << qApp->translate("cda-raw-writer", "Terminating application...")
            << msg::endmsg;

   // Clean up after ourselves:
   delete g_fwriter;
   delete g_crate;

   exit(0);

   return;
}
