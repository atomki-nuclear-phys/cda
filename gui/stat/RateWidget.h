// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_GUI_STAT_RATEWIDGET_H
#define CDA_GUI_STAT_RATEWIDGET_H

// STL include(s):
#include <list>

// Qt include(s):
#include <QtGui/QWidget>

namespace stat {

   using QT_PREPEND_NAMESPACE( QWidget );

   class RateWidget : public QWidget {

      Q_OBJECT

   public:
      RateWidget( QWidget* parent = 0, Qt::WindowFlags flags = 0 );
      ~RateWidget();

   public slots:
      void setNewRate( double rate );

   protected:
      static const quint32 MARK_HEIGHT;

      virtual void paintEvent( QPaintEvent* event );

   private:
      static const quint32 RATE_HISTORY_LENGTH;
      std::list< double > m_rateValues;

   }; // class RateWidget

} // namespace stat

#endif // CDA_GUI_STAT_RATEWIDGET_H
