
// Qt include(s):
#include <QtGui/QLabel>

// Local include(s):
#include "Gui.h"

namespace camac {

   Gui::Gui( QWidget* parent, Qt::WindowFlags flags )
      : dev::Gui( parent, flags ), m_logger( "Example::Gui" ) {

      m_label = new QLabel( "This is an example device",
                            this );
      m_label->setGeometry( QRect( 50, 50, 200, 50 ) );
      m_logger << msg::VERBOSE << "Object created" << msg::endmsg;

   }

   Gui::~Gui() {

      if( m_label ) delete m_label;
      m_logger << msg::VERBOSE << "Object deleted" << msg::endmsg;

   }

} // namespace camac
