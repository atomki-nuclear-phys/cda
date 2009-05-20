// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_GUI_STAT_RATEWIDGET_H
#define CDA_GUI_STAT_RATEWIDGET_H

// STL include(s):
#include <list>

// Qt include(s):
#include <QtGui/QWidget>

namespace stat {

   // Bring the Qt objects into this namespace:
   using QT_PREPEND_NAMESPACE( QWidget );

   /**
    *  @short Special widget showing a bar chart of rates
    *
    *         Since Qt didn't have a built-in widget that would've looked the way
    *         I imagined it, I created this custom widget for visualising the
    *         event processing rate of the various CDA applications.
    *
    *         It's very similar to how the CPU usage looks like in MacOS X's
    *         Activity Monitor.
    *
    *         The nice thing is, that the widget sets its vertical axis according
    *         to the rate values that it should be showing. Just like the
    *         Network Activity graph does in Activity Monitor...
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class RateWidget : public QWidget {

      Q_OBJECT

   public:
      /// Standard QWidget-style constructor
      RateWidget( QWidget* parent = 0, Qt::WindowFlags flags = 0 );

   public slots:
      /// Update the widget with a new rate value
      void setNewRate( qreal rate );

   protected:
      /// Height of the marks in the widget
      /**
       * The widget draws small rectangles with nearly fixed height. (So if
       * you increase the height of the widget, more marks will be shown.)
       * The reference height for the marks is defined by this constant.
       */
      static const quint32 MARK_HEIGHT;

      /// Re-implemented function, used to draw the widget
      virtual void paintEvent( QPaintEvent* event );

   private:
      /// "Memory length" of the widget
      /**
       * The widget remembers a certain (fixed) number of previous rate values.
       * This constant sets how many rate values should be remembered, and showed
       * at the same time by the widget.
       */
      static const quint32 RATE_HISTORY_LENGTH;
      /// Currently shown rate values
      /**
       * This variable stores the previous RATE_HISTORY_LENGTH number of rate
       * values. It is used when drawing the widget.
       */
      std::list< qreal > m_rateValues;

   }; // class RateWidget

} // namespace stat

#endif // CDA_GUI_STAT_RATEWIDGET_H
