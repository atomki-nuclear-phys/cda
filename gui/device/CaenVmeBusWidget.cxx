// $Id$

// Local include(s):
#include "CaenVmeBusWidget.h"

namespace dev {

   CaenVmeBusWidget::CaenVmeBusWidget( QWidget* parent,
                                       Qt::WindowFlags flags )
      : QWidget( parent, flags ) {

      // Set the size of the widget:
      setMinimumSize( WIDGET_WIDTH, WIDGET_HEIGHT );
      setMaximumSize( WIDGET_WIDTH, WIDGET_HEIGHT );

      // Set up the main label:
      m_label.reset( new QLabel( "CAEN VME Controller Configuration",
                                 this ) );
      m_label->setGeometry( QRect( 20, 20, 470, 40 ) );
      QFont font = m_label->font();
      font.setPointSize( 15 );
      m_label->setFont( font );
      m_label->setAlignment( Qt::AlignCenter );

      // Set up the controller selector label:
      m_typeLabel.reset( new QLabel( "Controller type:", this ) );
      m_typeLabel->setGeometry( QRect( 20, 100, 120, 25 ) );

      // Set up the controller selector:
      m_type.reset( new QComboBox( this ) );
      m_type->setGeometry( QRect( 150, 100, 300, 25 ) );
      m_type->addItem( tr( "V1718 USB-VME bridge" ) );
      m_type->addItem( tr( "V2718 PCI-VME bridge with optical link" ) );
      m_type->addItem( tr( "A2818 PCI board with optical link" ) );
      m_type->addItem( tr( "A2719 optical link piggy-back" ) );
      connect( m_type.get(), SIGNAL( currentIndexChanged( int ) ),
               this, SLOT( typeModifiedSlot( int ) ) );

      // Set up the link number selector label:
      m_linkNumberLabel.reset( new QLabel( "Link number:", this ) );
      m_linkNumberLabel->setGeometry( QRect( 20, 130, 120, 25 ) );

      // Set up the link number selector:
      m_linkNumber.reset( new QSpinBox( this ) );
      m_linkNumber->setGeometry( QRect( 150, 130, 100, 25 ) );
      m_linkNumber->setRange( 0, 255 );
      connect( m_linkNumber.get(), SIGNAL( valueChanged( int ) ),
               this, SLOT( linkNumberModifiedSlot( int ) ) );

      // Set up the board number selector label:
      m_boardNumberLabel.reset( new QLabel( "Board number:", this ) );
      m_boardNumberLabel->setGeometry( QRect( 20, 160, 120, 25 ) );

      // Set up the link number selector:
      m_boardNumber.reset( new QSpinBox( this ) );
      m_boardNumber->setGeometry( QRect( 150, 160, 100, 25 ) );
      m_boardNumber->setRange( 0, 255 );
      connect( m_boardNumber.get(), SIGNAL( valueChanged( int ) ),
               this, SLOT( boardNumberModifiedSlot( int ) ) );
   }

   caen::VmeBus::BoardType CaenVmeBusWidget::type() const {

      return static_cast< caen::VmeBus::BoardType >( m_type->currentIndex() );
   }

   void CaenVmeBusWidget::setType( caen::VmeBus::BoardType value ) {

      // Prevent the emission of signals while the setting is made:
      m_type->setEnabled( false );
      m_type->setCurrentIndex( static_cast< int >( value ) );
      m_type->setEnabled( true );

      return;
   }

   short CaenVmeBusWidget::linkNumber() const {

      return static_cast< short >( m_linkNumber->value() );
   }

   void CaenVmeBusWidget::setLinkNumber( short value ) {

      // Prevent the emission of signals while the setting is made:
      m_linkNumber->setEnabled( false );
      m_linkNumber->setValue( value );
      m_linkNumber->setEnabled( true );

      return;
   }

   short CaenVmeBusWidget::boardNumber() const {

      return static_cast< short >( m_boardNumber->value() );
   }

   void CaenVmeBusWidget::setBoardNumber( short value ) {

      // Prevent the emission of signals while the setting is made:
      m_boardNumber->setEnabled( false );
      m_boardNumber->setValue( value );
      m_boardNumber->setEnabled( true );

      return;
   }

   void CaenVmeBusWidget::reset() {

      setType( caen::VmeBus::BOARD_V1718 );
      setLinkNumber( 0 );
      setBoardNumber( 0 );

      return;
   }

   void CaenVmeBusWidget::typeModifiedSlot( int index ) {

      // Translate the index into a proper type:
      caen::VmeBus::BoardType type =
         static_cast< caen::VmeBus::BoardType >( index );
      // Emit the right signal:
      emit( typeModified( type ) );

      return;
   }

   void CaenVmeBusWidget::linkNumberModifiedSlot( int value ) {

      // Emit the right signal:
      emit( linkNumberModified( static_cast< short >( value ) ) );

      return;
   }

   void CaenVmeBusWidget::boardNumberModifiedSlot( int value ) {

      // Emit the right signal:
      emit( boardNumberModified( static_cast< short >( value ) ) );

      return;
   }

} // namespace dev
