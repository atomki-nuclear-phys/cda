// $Id$

// System include(s):
#include <cassert>

// Qt include(s):
#include <QtCore/QStringList>
#include <QStackedLayout>
#include <QSplitter>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/common/Address.h"
#   include "cdadaq/stat/Statistics.h"
#else
#   include "common/Address.h"
#   include "stat/Statistics.h"
#endif

// Local include(s):
#include "MDIView.h"
#include "RateWindow.h"

namespace cdastat {

   // Bring the Qt class definitions into this namespace:
   using QT_PREPEND_NAMESPACE( QStringList );
   using QT_PREPEND_NAMESPACE( QTableWidgetItem );
   using QT_PREPEND_NAMESPACE( QMdiSubWindow );

   /**
    * The constructor creates all the graphic widgets and sets up the objects
    * for handling the statistics information.
    */
   MDIView::MDIView( QWidget* parent, Qt::WindowFlags flags )
      : QWidget( parent, flags ) {

      // Create a simple layout for the top-level widget:
      m_layout = new QStackedLayout( this );

      // Create a QSplitter that displays the two main objects of the widget:
      m_splitter = new QSplitter( Qt::Vertical );
      m_splitter->setSizePolicy( QSizePolicy::Expanding,
                                 QSizePolicy::Expanding );
      m_layout->addWidget( m_splitter );

      // Create an MDI area showing the statistics information coming in from
      // various CDA applications in sub-windows:
      m_mdiArea = new QMdiArea();
      m_mdiArea->setMinimumSize( MINIMUM_WIDTH, MINIMUM_MDI_HEIGHT );
      m_mdiArea->setMaximumSize( 20000, 20000 );
      m_mdiArea->setSizePolicy( QSizePolicy::Expanding,
                                QSizePolicy::Expanding );
      m_splitter->addWidget( m_mdiArea );

      // Create a simple table that displays a quick overview of the event processing
      // rates of the whole CDA DAQ system:
      m_rateTable = new QTableWidget();
      m_rateTable->setColumnCount( 2 );
      m_rateTable->setRowCount( 0 );
      m_rateTable->setMinimumSize( MINIMUM_WIDTH, MINIMUM_TABLE_HEIGHT );
      m_rateTable->setMaximumSize( 20000, 20000 );
      m_rateTable->setSizePolicy( QSizePolicy::Expanding,
                                  QSizePolicy::Preferred );
      m_rateTable->setEditTriggers( QAbstractItemView::NoEditTriggers );
      m_rateTable->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded );
      m_rateTable->setHorizontalScrollMode( QAbstractItemView::ScrollPerPixel );
      m_rateTable->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
      m_rateTable->setVerticalScrollMode( QAbstractItemView::ScrollPerItem );
//      m_rateTable->horizontalHeader()->setMovable( false );
//      m_rateTable->horizontalHeader()->setResizeMode( QHeaderView::Fixed );
      m_rateTable->verticalHeader()->hide();

      QStringList headerLabels;
      headerLabels << tr( "Source" ) << tr( "Rate [Hz]" );
      m_rateTable->setHorizontalHeaderLabels( headerLabels );

      m_rateTable->setColumnWidth( 0, m_rateTable->viewport()->width() -
                                   TABLE_RATE_COLUMN_WIDTH );
      m_rateTable->setColumnWidth( 1, TABLE_RATE_COLUMN_WIDTH );

      m_splitter->addWidget( m_rateTable );
   }

   /**
    * The destructor deletes all the objects created by the widget.
    */
   MDIView::~MDIView() {

      delete m_mdiArea;
      delete m_rateTable;
      delete m_splitter;
      delete m_layout;
   }

   /**
    * This slot processes the incoming statistics information. If the information
    * came from a source that is already known, then it just gives the object to the
    * window handling the information from that source. If it's a new source, then
    * it creates a new stat::RateWindow and a new row in the rate table for it.
    * Then it lets the sub-window do its job with the statistics object...
    */
   void MDIView::handleStatistics( const Statistics& stat ) {

      // Check if we already know about this source:
      if( m_windowMap.contains( stat.getSource() ) ) {

         m_windowMap[ stat.getSource() ]->updateStatistics( stat );

      } else {

         // Create a new sub-window handling this source:
         RateWindow* window = new RateWindow( this );
         window->updateStatistics( stat );
         m_windowMap[ stat.getSource() ] = window;
         QMdiSubWindow* subwin = m_mdiArea->addSubWindow( window );
         subwin->setAttribute( Qt::WA_DeleteOnClose );
         subwin->setMinimumSize( window->width() + 20,
                                 window->height() + 30 );
         subwin->setMaximumSize( window->width() + 20,
                                 window->height() + 30 );

         // Connect the signals of this new sub-window:
         connect( window, SIGNAL( newRateAvailable( const QString&, qreal ) ),
                  this, SLOT( updateRateTable( const QString&, qreal ) ) );
         connect( window, SIGNAL( aboutToClose( const QString& ) ),
                  this, SLOT( handleWindowClose( const QString& ) ) );
         connect( subwin, SIGNAL( aboutToActivate() ),
                  window, SLOT( update() ) );

         // Show the sub-window:
         window->show();

         // Create a new row for this source in the rate table:
         QTableWidgetItem* source = new QTableWidgetItem( stat.getSource() );
         QTableWidgetItem* rate = new QTableWidgetItem( "0" );
         int row = m_rateTable->rowCount();
         m_rateTable->insertRow( row );
         m_rateTable->setItem( row, 0, source );
         m_rateTable->setItem( row, 1, rate );

      }

      return;
   }

   /**
    * The rate table uses a fixed geometry, which is updated in this function.
    *
    * @see adjustTableColumns()
    */
   void MDIView::showEvent( QShowEvent* ) {

      adjustTableColumns();
      return;
   }

   /**
    * The rate table uses a fixed geometry, which is updated in this function.
    *
    * @see adjustTableColumns()
    */
   void MDIView::resizeEvent( QResizeEvent* ) {

      adjustTableColumns();
      return;
   }

   /**
    * This slot is responsible for periodically updating the event processing rates in
    * the summary table.
    *
    * @param source Source ID of the rate information
    * @param rate The actual event processing rate (in Hz)
    */
   void MDIView::updateRateTable( const QString& source, qreal rate ) {

      // Find the row in the table showing information about this source:
      QList< QTableWidgetItem* > source_items =
         m_rateTable->findItems( source, Qt::MatchExactly );
      assert( source_items.size() == 1 );
      int row = m_rateTable->row( source_items.front() );

      // Update the rate value in the table:
      QTableWidgetItem* rate_item = m_rateTable->item( row, 1 );
      rate_item->setText( QString::number( rate ) );

      return;
   }

   /**
    * When a sub-window is closed, the associated table row is deleted from the rate
    * table by this function.
    *
    * @param source Source ID of the rate information
    */
   void MDIView::handleWindowClose( const QString& source ) {

      // Find the row in the table showing information about this source:
      QList< QTableWidgetItem* > source_items =
         m_rateTable->findItems( source, Qt::MatchExactly );
      assert( source_items.size() == 1 );
      int row = m_rateTable->row( source_items.front() );

      // Remove all references of this statistics source:
      m_rateTable->removeRow( row );
      m_windowMap.remove( source );

      return;
   }

   /**
    * This simple function adjusts the two columns of the rate table so that they fill
    * the available horizontal space.
    */
   void MDIView::adjustTableColumns() {

      m_rateTable->setColumnWidth( 0, m_rateTable->viewport()->width() -
                                   TABLE_RATE_COLUMN_WIDTH );
      m_rateTable->setColumnWidth( 1, TABLE_RATE_COLUMN_WIDTH );

      return;
   }

} // namespace cdastat
