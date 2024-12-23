
// Local include(s):
#include "AxisStyleAction.h"

namespace moni {

AxisStyleAction::AxisStyleAction(Histogram::AxisStyle style,
                                 const QString& text, QObject* parent)
    : QAction(text, parent), m_style(style) {

   setCheckable(true);
   connect(this, SIGNAL(triggered()), this, SLOT(triggeredSlot()));
}

void AxisStyleAction::triggeredSlot() {

   emit triggered(m_style);
   return;
}

}  // namespace moni
