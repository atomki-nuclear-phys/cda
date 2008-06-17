
// Qt include(s):
#include <QtCore/QString>
#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtGui/QSpinBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QLineEdit>

// Local include(s):
#include "Gui.h"

namespace camac {

   Gui::Gui( QWidget* parent, Qt::WindowFlags flags )
      : dev::Gui( parent, flags ), m_logger( "Example::Gui" ) {

      m_label = new QLabel( "This is an example device",
                            this );
      m_label->setGeometry( QRect( 50, 50, 200, 25 ) );

      m_booleanConfig = new QCheckBox( "Boolean property", this );
      m_booleanConfig->setGeometry( QRect( 50, 100, 200, 25 ) );
      connect( m_booleanConfig, SIGNAL( toggled( bool ) ),
               this, SLOT( booleanPropertyChangedSlot( bool ) ) );

      m_integerLabel = new QLabel( "Integer property:", this );
      m_integerLabel->setGeometry( QRect( 50, 150, 120, 25 ) );
      m_integerConfig = new QSpinBox( this );
      m_integerConfig->setGeometry( QRect( 170, 150, 120, 25 ) );
      connect( m_integerConfig, SIGNAL( valueChanged( int ) ),
               this, SLOT( integerPropertyChangedSlot( int ) ) );

      m_doubleLabel = new QLabel( "Double property:", this );
      m_doubleLabel->setGeometry( QRect( 50, 200, 120, 25 ) );
      m_doubleConfig = new QDoubleSpinBox( this );
      m_doubleConfig->setGeometry( QRect( 170, 200, 120, 25 ) );
      connect( m_doubleConfig, SIGNAL( valueChanged( double ) ),
               this, SLOT( doublePropertyChangedSlot( double ) ) );

      m_stringLabel = new QLabel( "String property:", this );
      m_stringLabel->setGeometry( QRect( 50, 250, 120, 25 ) );
      m_stringConfig = new QLineEdit( this );
      m_stringConfig->setGeometry( QRect( 170, 250, 200, 25 ) );
      connect( m_stringConfig, SIGNAL( textChanged( const QString& ) ),
               this, SLOT( stringPropertyChangedSlot( const QString& ) ) );

      m_logger << msg::VERBOSE << "Object created" << msg::endmsg;

   }

   Gui::~Gui() {

      if( m_label ) delete m_label;
      m_logger << msg::VERBOSE << "Object deleted" << msg::endmsg;

   }

   void Gui::booleanPropertyChangedSlot( bool value ) {

      m_booleanProperty = value;
      return;

   }

   void Gui::integerPropertyChangedSlot( int value ) {

      m_integerProperty = value;
      return;

   }

   void Gui::doublePropertyChangedSlot( double value ) {

      m_doubleProperty = value;
      return;

   }

   void Gui::stringPropertyChangedSlot( const QString& value ) {

      m_stringProperty = value;
      return;

   }

} // namespace camac
