// $Id$

// System include(s):
#include <cstdlib>
extern "C" {
#   include <sys/types.h>
#   include <dirent.h>
}

// Qt include(s):
#include <QtCore/QStringList>

// Local include(s):
#include "PathResolver.h"

namespace daq {

   //
   // Initialise the member variables:
   //
   std::map< QString, std::list< QString > > PathResolver::m_environment;
   msg::Logger PathResolver::m_logger( "daq::PathResolver" );

   /**
    * This is the main function of this class. It reads in the value of the
    * environment variable specified, and searches in the specified directories
    * for the given file name.
    *
    * The file name should not contain any slashes. If it does, the code
    * assumes that it's already a full path name and returns it unmodified.
    *
    * @returns The full path name of the file if successful, "" if the file
    *          could not be found, and the specified name itself if it
    *          contains any slashes
    */
   QString PathResolver::resolve( const QString& name, const QString& env ) {

      //
      // Access the cached environment variables. If it's not available
      // yet, then do the caching:
      //
      std::map< QString, std::list< QString > >::const_iterator envlist;
      if( ( envlist = m_environment.find( env ) ) != m_environment.end() ) {
         m_logger << msg::VERBOSE << tr( "Environment variable \"%1\" already cached" )
            .arg( env ) << msg::endmsg;
      } else {
         m_logger << msg::DEBUG << tr( "Caching environment: %1" ).arg( env )
                  << msg::endmsg;
         QString envarray = ::getenv( env.toLatin1().constData() );
         m_logger << msg::VERBOSE << tr( "%1 = %2" ).arg( env ).arg( envarray )
                  << msg::endmsg;
         QStringList envsplit = envarray.split( ":", QString::SkipEmptyParts );
         for( QStringList::const_iterator element = envsplit.begin();
              element != envsplit.end(); ++element ) {
            m_logger << msg::VERBOSE << tr( "Adding \"%1\" to environment %2" )
               .arg( *element ).arg( env ) << msg::endmsg;
            m_environment[ env ].push_back( *element );
         }
         envlist = m_environment.find( env );
      }

      //
      // We don't look for files in subdirectories:
      //
      if( name.contains( "/" ) ) {
         m_logger << msg::WARNING
                  << tr( "The specified executable name (%1) contains a slash." )
            .arg( name ) << std::endl
                  << tr( "Treating it as an absolute path..." ) << msg::endmsg;
         return name;
      }

      //
      // Loop over all the specified directories and see if they contain the
      // file that we're looking for:
      //
      for( std::list< QString >::const_iterator dir = envlist->second.begin();
           dir != envlist->second.end(); ++dir ) {

         //
         // This is something I've found on the web. Google does work...
         //
         DIR* d = ::opendir( dir->toLatin1().constData() );
         struct dirent* file;
         while( ( file = ::readdir( d ) ) != NULL ) {
            if( name == file->d_name ) {
               m_logger << msg::DEBUG << tr( "\"%1\" found in directory: %2" )
                  .arg( name ).arg( *dir ) << msg::endmsg;
               ::closedir( d );
               return ( *dir + "/" + name );
            }
         }
         ::closedir( d );

      }

      //
      // If we reached this point then we failed to find the file:
      //
      m_logger << msg::WARNING << tr( "Failed to find \"%1\" in environment: %2\n"
                                      "Assuming that it's in \"%3/bin\"" )
         .arg( name ).arg( env ).arg( CDASYS ) << msg::endmsg;

      return ( CDASYS + ( "/bin/" + name ) );
   }

} // namepsace daq
