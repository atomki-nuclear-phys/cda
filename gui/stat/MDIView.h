// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_GUI_STAT_MDIVIEW_H
#define CDA_GUI_STAT_MDIVIEW_H

// Qt include(s):
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtGui/QWidget>

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QStackedLayout )
QT_FORWARD_DECLARE_CLASS( QSplitter )
QT_FORWARD_DECLARE_CLASS( QMdiArea )
QT_FORWARD_DECLARE_CLASS( QTableWidget )

namespace stat {

   // Forward declaration(s):
   class Statistics;
   class Server;
   class RateWindow;

   // Bring the Qt class definitions into this namespace:
   using QT_PREPEND_NAMESPACE( QWidget );
   using QT_PREPEND_NAMESPACE( QStackedLayout );
   using QT_PREPEND_NAMESPACE( QSplitter );
   using QT_PREPEND_NAMESPACE( QMdiArea );
   using QT_PREPEND_NAMESPACE( QTableWidget );

   class MDIView : public QWidget {

      Q_OBJECT

   public:
      MDIView( QWidget* parent = 0, Qt::WindowFlags f = 0 );
      ~MDIView();

   protected:
      virtual void showEvent( QShowEvent* event );
      virtual void resizeEvent( QResizeEvent* event );

   private slots:
      void handleStatistics( const stat::Statistics& stat );
      void updateRateTable( const QString& source, double rate );
      void handleWindowClose( const QString& source );

   private:
      static const int MINIMUM_WIDTH;
      static const int MINIMUM_MDI_HEIGHT;
      static const int MINIMUM_TABLE_HEIGHT;
      static const int TABLE_RATE_COLUMN_WIDTH;

      void adjustTableColumns();

      QStackedLayout* m_layout;
      QSplitter* m_splitter;
      QMdiArea* m_mdiArea;
      QTableWidget* m_rateTable;

      Server* m_server;

      QMap< QString, RateWindow* > m_windowMap;

   }; // class MDIView

} // namespace stat

#endif // CDA_GUI_STAT_MDIVIEW_H
