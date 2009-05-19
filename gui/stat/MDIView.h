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
QT_FORWARD_DECLARE_CLASS( QMdiArea )

namespace stat {

   // Forward declaration(s):
   class Statistics;
   class Server;
   class RateWindow;

   // Bring the Qt class definitions into this namespace:
   using QT_PREPEND_NAMESPACE( QWidget );
   using QT_PREPEND_NAMESPACE( QStackedLayout );
   using QT_PREPEND_NAMESPACE( QMdiArea );

   class MDIView : public QWidget {

      Q_OBJECT

   public:
      MDIView( QWidget* parent = 0, Qt::WindowFlags f = 0 );
      ~MDIView();

   private slots:
      void handleStatistics( const stat::Statistics& stat );

   private:
      QStackedLayout* m_layout;
      QMdiArea* m_mdiArea;

      Server* m_server;

      QMap< QString, RateWindow* > m_windowMap;

   }; // class MDIView

} // namespace stat

#endif // CDA_GUI_STAT_MDIVIEW_H
