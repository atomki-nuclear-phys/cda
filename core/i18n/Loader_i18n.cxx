// $Id$

// Qt include(s):
#include <QtCore/QDir>
#include <QtCore/QStringList>
#include <QtCore/QFileInfo>
#include <QtCore/QTranslator>

// Local include(s):
#include "Loader.h"

namespace i18n {

   /// Flag specifying if the hungarian translations should be loaded or not
   static const bool USE_HUNGARIAN_TRANSLATIONS = true;

   // Make sure that the Qt classes are in scope:
   using QT_PREPEND_NAMESPACE( QDir );
   using QT_PREPEND_NAMESPACE( QStringList );
   using QT_PREPEND_NAMESPACE( QFileInfo );
   using QT_PREPEND_NAMESPACE( QTextCodec );

   Loader::Loader( const QString& path )
      : m_path( path ), m_logger( "i18n::Loader" ) {

      //
      // Check if a directory name has been specified:
      //
      if( m_path.isEmpty() ) {
         //
         // Get the CDASYS environment variable. Remember that the
         // return value is in our responsibility...
         //
         char* env_path = getenv( "CDASYS" );
         if( ! env_path ) {
            // In case CDASYS is not in the environment, try using the directory where
            // the code was compiled:
            m_path = CDASYS;
            m_path.append( "/trans" );
         } else {
            m_path = env_path;
            m_path.append( "/trans" );
         }
         m_logger << msg::DEBUG << tr( "Setting translation directory "
                                       "to: %1" ).arg( m_path ) << msg::endmsg;

         //
         // Delete the return value of getenv():
         //
         // Mac OS X complains about this call, so it's removed for the moment.
         //         if( env_path ) free( env_path );
      }

   }

   Loader::~Loader() {

      std::vector< QTranslator* >::iterator itr = m_translators.begin();
      std::vector< QTranslator* >::iterator end = m_translators.end();
      for( ; itr != end; ++itr ) {
         qApp->removeTranslator( *itr );
         delete *itr;
      }
      m_translators.clear();
   }

   bool Loader::loadTranslations() {

      // Return right away if we don't want to use the translations:
      if( ! USE_HUNGARIAN_TRANSLATIONS ) return true;

      //
      // Get the names of all physical files in the specified directory:
      //
      QDir pluginDir( m_path );
      QStringList files = pluginDir.entryList( QDir::Files | QDir::NoSymLinks |
                                               QDir::Readable );

      //
      // Loop over all file names:
      //
      for( QStringList::const_iterator pname = files.begin();
           pname != files.end(); ++pname ) {

         if( ! pname->contains( "_hu.qm" ) ) continue;

         QFileInfo finfo( *pname );
         QTranslator* trans = new QTranslator();
         if( ! trans->load( finfo.fileName(), m_path ) ) {
            m_logger << msg::ERROR
                     << tr( "Couldn't load translation file: %1" ).arg( finfo.fileName() )
                     << msg::endmsg;
            return false;
         }
         m_logger << msg::VERBOSE
                  << tr( "Loaded translation file: %1" ).arg( finfo.fileName() )
                  << msg::endmsg;

         qApp->installTranslator( trans );
         m_translators.push_back( trans );

      }

      m_logger << msg::DEBUG << tr( "Loaded all translations" ) << msg::endmsg;
      return true;
   }

} // namespace i18n
