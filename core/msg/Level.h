// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_MSG_LEVEL_H
#define CDA_CORE_MSG_LEVEL_H

namespace msg {

   /**
    *  @short "Type" of a message
    *
    *         This enumerator is used to label the type of a given
    *         message. The names of the types should speak for
    *         themselves.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   enum Level {
      VERBOSE = 1, /**< Type for the most detailed messages. Only for serious debugging. */
      DEBUG   = 2, /**< Type for debug messages. */
      INFO    = 3, /**< Type for normal information messages. */
      WARNING = 4, /**< Type for smaller problems. Data acquisition not affected. */
      ERROR   = 5, /**< Type for "real" problems. Data acquisition is affected. */
      FATAL   = 6, /**< Type for problems that should halt the data acquisition. */
      ALWAYS  = 7  /**< Type that should always be shown. No particular meaning. */
   }; // enum Level

} // namespace msg

#endif // CDA_CORE_MSG_LEVEL_H
