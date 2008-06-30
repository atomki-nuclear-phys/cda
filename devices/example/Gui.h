// Dear emacs, this is -*- c++ -*-
#ifndef CDA_DEVICES_EXAMPLE_GUI_H
#define CDA_DEVICES_EXAMPLE_GUI_H

// Qt include(s):
#include <QtCore/QGlobalStatic>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/Gui.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "device/Gui.h"
#   include "msg/Logger.h"
#endif

// Local include(s):
#include "Config.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QLabel )
QT_FORWARD_DECLARE_CLASS( QCheckBox )
QT_FORWARD_DECLARE_CLASS( QSpinBox )
QT_FORWARD_DECLARE_CLASS( QDoubleSpinBox )
QT_FORWARD_DECLARE_CLASS( QLineEdit )

namespace camac {

   using QT_PREPEND_NAMESPACE( QLabel );
   using QT_PREPEND_NAMESPACE( QCheckBox );
   using QT_PREPEND_NAMESPACE( QSpinBox );
   using QT_PREPEND_NAMESPACE( QDoubleSpinBox );
   using QT_PREPEND_NAMESPACE( QLineEdit );

   class Gui : virtual public dev::Gui,
               virtual public Config {

      Q_OBJECT

   public:
      Gui( QWidget* parent = 0, Qt::WindowFlags flags = 0 );
      ~Gui();

   private slots:
      void booleanPropertyChangedSlot( bool value );
      void integerPropertyChangedSlot( int value );
      void doublePropertyChangedSlot( double value );
      void stringPropertyChangedSlot( const QString& value );

   private:
      QLabel*         m_label;
      QCheckBox*      m_booleanConfig;
      QLabel*         m_integerLabel;
      QSpinBox*       m_integerConfig;
      QLabel*         m_doubleLabel;
      QDoubleSpinBox* m_doubleConfig;
      QLabel*         m_stringLabel;
      QLineEdit*      m_stringConfig;

      mutable msg::Logger m_logger;

   }; // class Gui

} // namespace camac

#endif // CDA_DEVICES_EXAMPLE_GUI_H
