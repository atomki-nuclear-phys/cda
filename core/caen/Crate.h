// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_COMMON_CAEN_CRATE_H
#define CDA_COMMON_CAEN_CRATE_H

// Qt include(s):
#include <QtCore/QString>

// Local include(s):
#include "../device/Crate.h"

namespace caen {

   /**
    *  @short Specialized "crate" class for CAEN devices
    *
    *         CAEN devices can be accessed in a few different ways.
    *         By using this class, one gets the capability of keeping
    *         track of the specific options needed to access the
    *         CAEN device(s).
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   template< class DEVICE >
   class Crate : public dev::Crate< DEVICE > {

   public:
      /// Default constructor
      Crate();

      /// Get the connection type for this crate
      Digitizer::ConnectionType getConnType() const;
      /// Set the connection type for this crate
      void setConnType( Digitizer::ConnectionType type );

   protected:
      /// Read the crate specific options from binary input
      virtual bool readCrateConfig( QIODevice* dev );
      /// Write the crate specific options to binary output
      virtual bool writeCrateConfig( QIODevice* dev ) const;

      /// Read the crate specific options from XML input
      virtual bool readCrateConfig( const QDomElement& node );
      /// Write the crate specific options to XML output
      virtual bool writeCrateConfig( QDomElement& node ) const;

      /// Connection type to the digitizer
      Digitizer::ConnectionType m_connType;

   private:
      /// Function converting the connection type to a primitive
      static quint32 convert( Digitizer::ConnectionType type );
      /// Function creating a connection type from a primitive
      static Digitizer::ConnectionType convert( quint32 type );
      /// Function converting the connection type to a string
      static QString toString( Digitizer::ConnectionType type );
      /// Function creating a connection type from a string
      static Digitizer::ConnectionType
      fromString( const QString& type );

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class Crate

} // namespace caen

// Include the implementation:
#include "Crate.icc"

#endif // CDA_COMMON_CAEN_CRATE_H
