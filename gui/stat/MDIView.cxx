// $Id$

#include <iostream>
#include <cassert>

// Qt include(s):
#include <QtCore/QStringList>
#include <QtGui/QStackedLayout>
#include <QtGui/QSplitter>
#include <QtGui/QMdiArea>
#include <QtGui/QMdiSubWindow>
#include <QtGui/QTableWidget>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QHeaderView>

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

   using QT_PREPEND_NAMESPACE( QTableWidgetItem );

   const int MDIView::MINIMUM_WIDTH = 500;
   const int MDIView::MINIMUM_MDI_HEIGHT = 350;
   const int MDIView::MINIMUM_TABLE_HEIGHT = 150;
   const int MDIView::TABLE_RATE_COLUMN_WIDTH = 130;

   MDIView::MDIView( QWidget* parent, Qt::WindowFlags f )
      : QWidget( parent, f ) {

      m_layout = new QStackedLayout( this );

      m_splitter = new QSplitter( Qt::Vertical );
      m_splitter->setSizePolicy( QSizePolicy::Expanding,
                                 QSizePolicy::Expanding );
      m_layout->addWidget( m_splitter );

      m_mdiArea = new QMdiArea();
      m_mdiArea->setMinimumSize( MINIMUM_WIDTH, MINIMUM_MDI_HEIGHT );
      m_mdiArea->setMaximumSize( 20000, 20000 );
      m_mdiArea->setSizePolicy( QSizePolicy::Expanding,
                                QSizePolicy::Expanding );

      m_splitter->addWidget( m_mdiArea );

      m_rateTable = new QTableWidget();
      m_rateTable->setColumnCount( 2 );
      m_rateTable->setRowCount( 0 );
      m_rateTable->setMinimumSize( MINIMUM_WIDTH, MINIMUM_TABLE_HEIGHT );
      m_rateTable->setMaximumSize( 20000, 20000 );
      m_rateTable->setSizePolicy( QSizePolicy::Expanding,
                                  QSizePolicy::Preferred );
      m_rateTable->setEditTriggers( QAbstractItemView::NoEditTriggers );
      //      m_rateTable->setSelectionMode( QAbstractItemView::NoSelection );
      m_rateTable->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded );
      m_rateTable->setHorizontalScrollMode( QAbstractItemView::ScrollPerPixel );
      m_rateTable->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
      m_rateTable->setVerticalScrollMode( QAbstractItemView::ScrollPerItem );
      m_rateTable->horizontalHeader()->setMovable( false );
      m_rateTable->horizontalHeader()->setResizeMode( QHeaderView::Fixed );
      m_rateTable->verticalHeader()->hide();

      QStringList headerLabels;
      headerLabels << "Source" << "Rate [Hz]";
      m_rateTable->setHorizontalHeaderLabels( headerLabels );

      m_rateTable->setColumnWidth( 0, m_rateTable->viewport()->width() -
                                   TABLE_RATE_COLUMN_WIDTH );
      m_rateTable->setColumnWidth( 1, TABLE_RATE_COLUMN_WIDTH );

      m_splitter->addWidget( m_rateTable );

      m_server = new Server( this );
      connect( m_server, SIGNAL( statAvailable( const stat::Statistics& ) ),
               this, SLOT( handleStatistics( const stat::Statistics& ) ) );
      m_server->listen( Address( "0.0.0.0", 35000 ) );

   }

   MDIView::~MDIView() {

      delete m_mdiArea;
      delete m_layout;

   }

   void MDIView::showEvent( QShowEvent* ) {

      adjustTableColumns();
      return;

   }

   void MDIView::resizeEvent( QResizeEvent* ) {

      adjustTableColumns();
      return;

   }

   void MDIView::handleStatistics( const Statistics& stat ) {

      if( m_windowMap.contains( stat.getSource() ) ) {
         m_windowMap[ stat.getSource() ]->updateStatistics( stat );
      } else {

         RateWindow* window = new RateWindow( this );
         window->updateStatistics( stat );
         m_windowMap[ stat.getSource() ] = window;
         QMdiSubWindow* subwin = m_mdiArea->addSubWindow( window );
         subwin->setAttribute( Qt::WA_DeleteOnClose );
         subwin->setMinimumSize( window->width() + 20,
                                 window->height() + 30 );
         subwin->setMaximumSize( window->width() + 20,
                                 window->height() + 30 );

         connect( window, SIGNAL( newRateAvailable( const QString&, double ) ),
                  this, SLOT( updateRateTable( const QString&, double ) ) );
         connect( window, SIGNAL( aboutToClose( const QString& ) ),
                  this, SLOT( handleWindowClose( const QString& ) ) );

         window->show();

         QTableWidgetItem* source = new QTableWidgetItem( stat.getSource() );
         QTableWidgetItem* rate = new QTableWidgetItem( "0" );
         int row = m_rateTable->rowCount();
         m_rateTable->insertRow( row );
         m_rateTable->setItem( row, 0, source );
         m_rateTable->setItem( row, 1, rate );

      }

      return;

   }

   void MDIView::updateRateTable( const QString& source, double rate ) {

      QList< QTableWidgetItem* > source_items =
         m_rateTable->findItems( source, Qt::MatchExactly );
      assert( source_items.size() == 1 );

      int row = m_rateTable->row( source_items.front() );
      QTableWidgetItem* rate_item = m_rateTable->item( row, 1 );
      rate_item->setText( QString::number( rate ) );

      return;

   }

   void MDIView::handleWindowClose( const QString& source ) {

      QList< QTableWidgetItem* > source_items =
         m_rateTable->findItems( source, Qt::MatchExactly );
      assert( source_items.size() == 1 );

      int row = m_rateTable->row( source_items.front() );
      m_rateTable->removeRow( row );

      m_windowMap.remove( source );

      return;

   }

   void MDIView::adjustTableColumns() {

      m_rateTable->setColumnWidth( 0, m_rateTable->viewport()->width() -
                                   TABLE_RATE_COLUMN_WIDTH );
      m_rateTable->setColumnWidth( 1, TABLE_RATE_COLUMN_WIDTH );

      return;

   }

} // namespace stat
