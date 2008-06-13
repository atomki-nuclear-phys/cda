// Dear emacs, this is -*- c++ -*-
#ifndef CDA_DEVICES_EXAMPLE_GUI_H
#define CDA_DEVICES_EXAMPLE_GUI_H

// CDA include(s):
#include "device/Gui.h"
#include "msg/Logger.h"

// Local include(s):
#include "Config.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QLabel )

namespace camac {

   using QT_PREPEND_NAMESPACE( QLabel );

   class Gui : virtual public dev::Gui,
               virtual public Config {

      Q_OBJECT

   public:
      Gui( QWidget* parent = 0, Qt::WindowFlags flags = 0 );
      ~Gui();

   private:
      QLabel* m_label;
      mutable msg::Logger m_logger;

   }; // class Gui

} // namespace camac

#endif // CDA_DEVICES_EXAMPLE_GUI_H
