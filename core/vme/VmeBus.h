// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef COMMON_VME_VMEBUS_H
#define COMMON_VME_VMEBUS_H

// System include(s):
#include <stdint.h>

// Qt include(s):
#include <QtCore/QString>

// CDA include(s):
#include "../msg/Logger.h"
#include "../common/Export.h"

namespace vme {

   /// Convenience type definition for the VME addresses
   typedef unsigned int Address_t;

   /**
    *  @short Wrapper class around the VME device driver
    *
    *         This class is used to provide a common, driver independent
    *         interface to accessing the VME bus. It can currently use
    *         two types of device drivers: ARWVME and PCIVME. In addition
    *         it has a TESTING mode, in which it generates data similar to
    *         what a real VME device would provide.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class CDACORE_EXPORT VmeBus {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( vme::VmeBus )

   public:
      /// Default constructor
      VmeBus( const char* dev_path = "/dev/vmemm_1" );
      /// Default destructor
      ~VmeBus();

      /// Function that opens the connection to the driver
      int open( const char* dev_path = 0 );
      /// Function that closes the connection to the driver
      int close();
      /// Function returning the VME bus's file descriptor
      int fd() const;

      /// Check if the bus has been opened or not
      bool isOpen() const;

      /// Overloaded function that writes an 8 bit variable
      int write( Address_t address, char data );
      /// Overloaded function that writes a 16 bit variable
      int write( Address_t address, int16_t data );
      /// Overloaded function that writes a 32 bit variable
      int write( Address_t address, int32_t data );
      /// General function that writes a variable of a user defined size
      int write( Address_t address, void* data, unsigned int length );

      /// Overloaded function that reads an 8 bit variable
      int read( Address_t address, char* data ) const;
      /// Overloaded function that reads a 16 bit variable
      int read( Address_t address, int16_t* data ) const;
      /// Overloaded function that reads a 32 bit variable
      int read( Address_t address, int32_t* data ) const;
      /// General function that reads a variable of a user defined size
      int read( Address_t address, void* data, unsigned int length ) const;

      /// Function that puts the program in idle state for a short time
      static void wait();

   private:
      /// The VME file descriptor.
      int m_vmeFD;

      /// Default path to the VME bus device
      QString m_path;

#ifdef HAVE_VME_LIB
      /// VME bus address modifier of the PCIVME driver.
      /**
       * With the pcivme module, the data transfer setup is done in
       * more steps. This is the VME address modifier. I have to admit even I don't
       * fully understand it's role. I experimentally figured out a way in which data
       * writing and readout works, and I'm using that all the time...
       */
      unsigned char m_addressModifier;
#endif // HAVE_VME_LIB

      /// Variable signaling the device's state.
      /**
       * Maybe it's not the prettiest, but it's the easyest to store the
       * object's state in such a variable. It's only used inside the class,
       * so I think it's quite safe.
       */
      bool m_opened;

      /// Message logger object
      mutable msg::Logger m_logger;

   }; // class VmeBus

} // namespace vme

#endif // COMMON_VME_VMEBUS_H
