
// Qt include(s):
#include <QIODevice>
#include <QString>
#include <QByteArray>
#include <QCoreApplication>

// Local include(s):
#include "TextStream.h"
#include "Event.h"
#include "Fragment.h"

namespace ev {

   TextStream::TextStream( QIODevice* device )
      : QTextStream( device ) {

   }

   TextStream::TextStream( FILE* fileHandle )
      : QTextStream( fileHandle ) {

   }

   TextStream::TextStream( QString* string )
      : QTextStream( string ) {

   }

   TextStream::TextStream( QByteArray* array )
      : QTextStream( array ) {

   }

   /**
    * This operator prints a whole Event object. Since the Event class
    * is basically just a collection of Fragment objects, it just loops
    * over all the fragments and calls the appropriate output operator
    * for each of them.
    *
    * @param event The event you want to print
    * @returns This same stream object
    */
   TextStream& TextStream::operator<< ( const Event& event ) {

      ( * ( QTextStream* ) this )
         << qApp->translate( "ev::TextStream",
                             "-------- Event record --------\n" );
      ( * ( QTextStream* ) this )
         << qApp->translate( "ev::TextStream",
                             " Number of fragments: %1\n\n" )
         .arg( event.getFragments().size() );

      if( event.getFragments().size() ) {
         for( auto& fragment : event.getFragments() ) {
            *this << *fragment << "\n";
         }
      } else {
         ( * ( QTextStream* ) this )
            << qApp->translate( "ev::TextStream",
                                " No event fragments!\n" );
      }

      ( * ( QTextStream* ) this ) << "------------------------------";

      return *this;
   }

   /**
    * This operator prints most of the information. It prints the
    * location of the module that sent the fragment, then it prints all
    * the data words that the module sent.
    *
    * @param fragment The event fragment that is to be printed
    * @returns This same stream object
    */
   TextStream& TextStream::operator<< ( const Fragment& fragment ) {

      ( * ( QTextStream* ) this )
         << qApp->translate( "ev::TextStream",
                             "  ---- Fragment record ----\n" );
      ( * ( QTextStream* ) this )
         << qApp->translate( "ev::TextStream",
                             "   Module ID   : %1\n\n" )
         .arg( fragment.getModuleID() );

      if( fragment.getDataWords().size() ) {
         std::vector< uint32_t >::const_iterator itr =
            fragment.getDataWords().begin();
         std::vector< uint32_t >::const_iterator end =
            fragment.getDataWords().end();
         for( ; itr != end; ++itr ) {
            ( * ( QTextStream* ) this )
               << qApp->translate( "ev::TextStream",
                                   "   Data word: 0x%1\n" )
                  .arg( QString::number( *itr, 16 ) );
         }
      } else {
         ( * ( QTextStream* ) this )
            << qApp->translate( "ev::TextStream",
                                "   No data words!\n" );
      }

      ( * ( QTextStream* ) this ) << "  -------------------------";

      return *this;
   }

   /**
    * ev::TextStream is not supposed to provide all the functionalities of
    * QTextStream, but this operator seemed to be quite important to have here
    * as well...
    *
    * @param text A QString that should be appended to the output
    * @returns This same stream object
    */
   TextStream& TextStream::operator<<( const QString& text ) {

      QTextStream::operator<<( text );
      return *this;
   }

} // namespace ev
