// Dear emacs, this is -*- c++ -*-
#ifndef CDA_CORE_I18N_LOADER_H
#define CDA_CORE_I18N_LOADER_H

// STL include(s):
#include <vector>

// Qt include(s):
#include <QCoreApplication>
#include <QString>

// CDA include(s):
#include "../msg/Logger.h"
#include "../common/Export.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QTranslator )

/// Namespace for all the internationalization handling classes
namespace i18n {

   // Make sure that the Qt classes are in scope:
   using QT_PREPEND_NAMESPACE( QString );
   using QT_PREPEND_NAMESPACE( QTranslator );

   /**
    *  @short Class that can be used to load all CDA translation files
    *
    *         This class can be used by all the CDA applications to load the
    *         (hungarian) translation files before starting to print messages
    *         for the user.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    */
   class CDACORE_EXPORT Loader {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( i18n::Loader )

   public:
      /// Constructor specifying the directory of the translations
      Loader( const QString& path = "" );
      /// Destructor
      ~Loader();

      /// Function loading all the available translations
      bool loadTranslations();

   private:
      QString                     m_path;        ///< Translation directory
      std::vector< QTranslator* > m_translators; ///< The loaded translators
      mutable msg::Logger         m_logger;      ///< Private message logger object

   }; // class Loader

} // namespace i18n

#endif // CDA_CORE_I18N_LOADER_H
