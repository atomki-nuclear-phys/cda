// Dear emacs, this is -*- c++ -*-
#ifndef CDA_CORE_MSG_LEVEL_H
#define CDA_CORE_MSG_LEVEL_H

// For when building against the CAEN libraries on Windows with MSVC:
#ifdef ERROR
#undef ERROR
#endif

namespace msg {

/**
 *  @short "Type" of a message
 *
 *         This enumerator is used to label the type of a given
 *         message. The names of the types should speak for
 *         themselves.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */
enum Level {
   /// Type for the most detailed messages. Only for serious debugging.
   VERBOSE = 1,
   /// Type for debug messages.
   DEBUG = 2,
   /// Type for normal information messages.
   INFO = 3,
   /// Type for smaller problems. Data acquisition not affected.
   WARNING = 4,
   /// Type for "real" problems. Data acquisition is affected.
   ERROR = 5,
   /// Type for problems that should halt the data acquisition.
   FATAL = 6,
   /// Type that should always be shown. No particular meaning.
   ALWAYS = 7
};  // enum Level

}  // namespace msg

#endif  // CDA_CORE_MSG_LEVEL_H
