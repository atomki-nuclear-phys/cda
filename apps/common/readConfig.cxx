
// Local include(s).
#include "readConfig.h"

// Project include(s).
#include "common/Address.h"
#include "common/errorcheck.h"
#include "config/ConfReader.h"
#include "msg/Logger.h"

// Qt include(s).
#include <QFile>
#include <QDomDocument>

namespace apps
{
   StatusCode readConfig(const QString &source, dev::IConfig &device)
   {
      // Logger object.
      msg::Logger m_logger("apps::readConfig");

      //
      // Decide how to read the configuration.
      //
      if (Address::isAddress(source))
      {
         //
         // Read the configuration data from the specified address.
         //
         conf::ConfReader reader;
         CHECK(reader.readFrom(Address(source)));

         //
         // Initialise the object from the buffer.
         //
         CHECK(device.readConfig(reader.buffer()));
      }
      else
      {
         //
         // Open the configuration file:
         //
         QFile file(source);
         if (!file.open(QFile::ReadOnly | QFile::Text))
         {
            REPORT_ERROR(qApp->translate("apps::readConfig",
                                         "The specified configuration file "
                                         "(\"%1\") could not be opened!")
                             .arg(source));
            return StatusCode::FAILURE;
         }

         //
         // Read the file's contents into XML format:
         //
         QDomDocument doc;
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
         const auto result = doc.setContent(&file);
         if (!result)
         {
            REPORT_ERROR(qApp->translate("apps::readConfig", "Error in parsing \"%1\"\n"
                                                             "  Error message: %2\n"
                                                             "  Error line   : %3\n"
                                                             "  Error column : %4")
                             .arg(source)
                             .arg(result.errorMessage)
                             .arg(result.errorLine)
                             .arg(result.errorColumn));
#else
         QString errorMsg;
         int errorLine, errorColumn;
         if (!doc.setContent(&file, false, &errorMsg, &errorLine,
                             &errorColumn))
         {
            REPORT_ERROR(qApp->translate("apps::readConfig", "Error in parsing \"%1\"\n"
                                                             "  Error message: %2\n"
                                                             "  Error line   : %3\n"
                                                             "  Error column : %4")
                             .arg(source)
                             .arg(errorMsg)
                             .arg(errorLine)
                             .arg(errorColumn));
#endif
            return StatusCode::FAILURE;
         }
         else
         {
            REPORT_VERBOSE(qApp->translate("apps::readConfig", "Successfully parsed: %1")
                               .arg(source));
         }

         //
         // Initialise the object from the XML document.
         //
         CHECK(device.readConfig(doc.documentElement()));
      }

      // Let the user know what happened.
      m_logger << msg::INFO
               << qApp->translate("apps::readConfig",
                                  "Configuration successfully read from \"%1\"")
                      .arg(source)
               << msg::endmsg;

      // Return gracefully.
      return StatusCode::SUCCESS;
   }

} // namespace dev
