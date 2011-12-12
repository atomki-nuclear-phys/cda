// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_GUI_COMMON_DEFAULTFONT_H
#define CDA_GUI_COMMON_DEFAULTFONT_H

// Qt include(s):
#include <QtGui/QFont>

namespace gui {

   /**
    *  @short Default font for the CDA GUI applications
    *
    *         Since the font picked by CDA on Scientific Linux is beyond
    *         ugly, the application is forced to use this font in all the
    *         graphical interfaces.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class DefaultFont : public QFont {

   public:
      /// Constructor setting all default properties
      DefaultFont();

   }; // class DefaultFont

} // namespace gui

#endif // CDA_GUI_COMMON_DEFAULTFONT_H
