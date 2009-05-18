// $Id$

// Qt include(s):
#include <QtGui/QStackedLayout>
#include <QtGui/QMdiArea>

// Local include(s):
#include "MDIView.h"

namespace stat {

   MDIView::MDIView( QWidget* parent, Qt::WindowFlags f )
      : QWidget( parent, f ) {

      m_layout = new QStackedLayout( this );

      m_mdiArea = new QMdiArea();
      m_mdiArea->setMinimumSize( 450, 250 );
      m_mdiArea->setMaximumSize( 20000, 20000 );
      m_mdiArea->setSizePolicy( QSizePolicy::Expanding,
                                QSizePolicy::Expanding );

      m_layout->addWidget( m_mdiArea );

   }

   MDIView::~MDIView() {

      delete m_mdiArea;
      delete m_layout;

   }

} // namespace stat
