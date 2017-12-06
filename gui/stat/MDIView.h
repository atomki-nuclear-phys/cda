// Dear emacs, this is -*- c++ -*-
#ifndef CDA_GUI_STAT_MDIVIEW_H
#define CDA_GUI_STAT_MDIVIEW_H

// Qt include(s):
#include <QMap>
#include <QString>
#include <QWidget>

// Local include(s):
#include "../common/Export.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QStackedLayout )
QT_FORWARD_DECLARE_CLASS( QSplitter )
QT_FORWARD_DECLARE_CLASS( QMdiArea )
QT_FORWARD_DECLARE_CLASS( QTableWidget )

namespace cdastat {

   // Forward declaration(s):
   class Statistics;
   class RateWindow;

   // Bring the Qt class definitions into this namespace:
   using QT_PREPEND_NAMESPACE( QMap );
   using QT_PREPEND_NAMESPACE( QString );
   using QT_PREPEND_NAMESPACE( QWidget );
   using QT_PREPEND_NAMESPACE( QStackedLayout );
   using QT_PREPEND_NAMESPACE( QSplitter );
   using QT_PREPEND_NAMESPACE( QMdiArea );
   using QT_PREPEND_NAMESPACE( QTableWidget );

   /**
    *  @short Widget displaying statistics information from many CDA processes
    *
    *         This widget can be used to display statistics information from
    *         any number of CDA processes running on various computers on the
    *         network. It listens to incoming statistics from the network,
    *         and distributes this statistics to stat::RateWindow objects
    *         presenting the results.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    */
   class CDAGUI_EXPORT MDIView : public QWidget {

      Q_OBJECT

   public:
      /// Standard QWidget-style constructor
      MDIView( QWidget* parent = 0, Qt::WindowFlags flags = 0 );
      /// Destructor
      ~MDIView();

   public slots:
      /// Slot handling incoming statistics information
      void handleStatistics( const cdastat::Statistics& stat );

   protected:
      /// Re-implemented function for layout management
      virtual void showEvent( QShowEvent* event );
      /// Re-implemented function for layout management
      virtual void resizeEvent( QResizeEvent* event );

   private slots:
      /// Internal slot updating the rate table with new rate results
      void updateRateTable( const QString& source, qreal rate );
      /// Internal slot handling the closing of sub-windows
      void handleWindowClose( const QString& source );

   private:
      static const int MINIMUM_WIDTH = 500;
      static const int MINIMUM_MDI_HEIGHT = 350;
      static const int MINIMUM_TABLE_HEIGHT = 150;
      static const int TABLE_RATE_COLUMN_WIDTH = 130;

      /// Function adjusting the layout of the rate table
      void adjustTableColumns();

      QStackedLayout* m_layout; ///< Simple layout for the one top-level widget
      QSplitter* m_splitter; ///< Object holding the two child widgets of this object
      QMdiArea* m_mdiArea; ///< An MDI area showing sub-windows with processing rates
      QTableWidget* m_rateTable; ///< Table giving a quick overview of processing rates

      /// Map associating statistics source IDs with sub-windows showing their information
      QMap< QString, RateWindow* > m_windowMap;

   }; // class MDIView

} // namespace cdastat

#endif // CDA_GUI_STAT_MDIVIEW_H
