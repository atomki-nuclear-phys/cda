// Dear emacs, this is -*- c++ -*-
#ifndef CDA_GUI_MONI_AXISSTYLEACTION_H
#define CDA_GUI_MONI_AXISSTYLEACTION_H

// Qt include(s):
#include <QString>
#include <QAction>

// Local include(s):
#include "Histogram.h"
#include "../common/Export.h"

namespace moni {

   // Bring the Qt class(es) into scope:
   using QT_PREPEND_NAMESPACE( QString );
   using QT_PREPEND_NAMESPACE( QAction );

   /**
    *  @short Action modifying the style of one of the axes
    *
    *         This "action" is used internally by the pop-up menu of the
    *         Histogram class to change the style of one of its axes.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    */
   class CDADAQ_EXPORT AxisStyleAction : public QAction {

      Q_OBJECT

   public:
      /// Constructor specifying the style that should be changed to
      AxisStyleAction( Histogram::AxisStyle style,
                       const QString& text, QObject* parent = 0 );

   signals:
      /// Signal emitted when the action is called
      /**
       * @param style The style that should be changed to
       */
      void triggered( Histogram::AxisStyle style );

   private slots:
      /// Internal slot for emitting the triggered() signal
      void triggeredSlot();

   private:
      /// Style that should be changed to
      Histogram::AxisStyle m_style;

   }; // class AxisStyleAction

} // namespace moni

#endif // CDA_GUI_MONI_AXISSTYLEACTION_H
