// $Id$

// Qt include(s):
#include <QtGui/QStackedLayout>
#include <QtGui/QMdiArea>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/common/Address.h"
#   include "cdadaq/stat/Server.h"
#else
#   include "common/Address.h"
#   include "stat/Server.h"
#endif

// Local include(s):
#include "MDIView.h"
#include "RateWindow.h"

namespace stat {

   MDIView::MDIView( QWidget* parent, Qt::WindowFlags f )
      : QWidget( parent, f ) {

      m_layout = new QStackedLayout( this );

      m_mdiArea = new QMdiArea();
      m_mdiArea->setMinimumSize( 500, 350 );
      m_mdiArea->setMaximumSize( 20000, 20000 );
      m_mdiArea->setSizePolicy( QSizePolicy::Expanding,
                                QSizePolicy::Expanding );

      m_layout->addWidget( m_mdiArea );

      m_server = new Server( this );
      connect( m_server, SIGNAL( statAvailable( const stat::Statistics& ) ),
               this, SLOT( handleStatistics( const stat::Statistics& ) ) );
      m_server->listen( Address( "0.0.0.0", 35000 ) );

   }

   MDIView::~MDIView() {

      delete m_mdiArea;
      delete m_layout;

   }

   void MDIView::handleStatistics( const Statistics& stat ) {

      if( m_windowMap.contains( stat.getSource() ) ) {
         m_windowMap[ stat.getSource() ]->updateStatistics( stat );
      } else {

         RateWindow* window = new RateWindow( this );
         window->updateStatistics( stat );
         m_windowMap[ stat.getSource() ] = window;
         m_mdiArea->addSubWindow( window );
         window->show();

      }

      return;

   }

} // namespace stat
