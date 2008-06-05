// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_DEVICE_GUI_H
#define CDA_CORE_DEVICE_GUI_H

// Qt include(s):
#include <QtGui/QWidget>

// Local include(s):
#include "Config.h"

namespace dev {

   //
   // Make sure that the following Qt classes are available in the
   // DEV namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QWidget );

   /**
    *  @short Base class for the graphical representation of a device
    *
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class Gui : virtual public QWidget,
               virtual public Config {

      Q_OBJECT

   public:
      /// Standard QWidget style constructor
      Gui( QWidget* parent = 0, Qt::WindowFlags flags = 0 );
      /// Destructor
      virtual ~Gui();

      /// Fixed width of a device
      static const int WIDGET_WIDTH;
      /// Fixed height of a device
      static const int WIDGET_HEIGHT;

   }; // class Gui

} // namespace dev

#endif // CDA_CORE_DEVICE_GUI_H
