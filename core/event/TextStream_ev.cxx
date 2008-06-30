// $Id$

// Qt include(s):
#include <QtCore/QIODevice>
#include <QtCore/QString>
#include <QtCore/QByteArray>

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

      ( * ( QTextStream* ) this ) << "-------- Event record "
                                  << "--------\n";
      ( * ( QTextStream* ) this ) << " Number of fragments: "
                                  << event.getFragments().size() << "\n\n";

      if( event.getFragments().size() ) {
         for( std::vector< Fragment >::const_iterator fragment =
                 event.getFragments().begin();
              fragment != event.getFragments().end(); ++fragment ) {
            *this << *fragment << "\n";
         }
      } else {
         ( * ( QTextStream* ) this ) << " No event fragments!\n";
      }

      ( * ( QTextStream* ) this ) << "----------------"
                                  << "--------------";

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

      ( * ( QTextStream* ) this ) << "  ---- Fragment record ----\n";
      ( * ( QTextStream* ) this ) << "   Crate number : "
                                  << fragment.getCrateNumber() << "\n";
      ( * ( QTextStream* ) this ) << "   Module number: "
                                  << fragment.getModuleNumber() << "\n\n";

      if( fragment.getDataWords().size() ) {
         setIntegerBase( 16 );
         for( std::vector< uint32_t >::const_iterator dword =
                 fragment.getDataWords().begin();
              dword != fragment.getDataWords().end(); ++dword ) {
            ( * ( QTextStream* ) this ) << "   Data word: 0x"
                                        << *dword << "\n";
         }
         reset();
      } else {
         ( * ( QTextStream* ) this ) << "   No data words!\n";
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
