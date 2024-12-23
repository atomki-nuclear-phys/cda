
// Local include(s):
#include "CaenVmeGui.h"

namespace dev {

CaenVmeGui::CaenVmeGui(QWidget* parent, Qt::WindowFlags flags)
    : QWidget(parent, flags) {

   this->resize(WIDGET_WIDTH, WIDGET_HEIGHT);
   this->setMinimumSize(WIDGET_WIDTH, WIDGET_HEIGHT);
   this->setMaximumSize(WIDGET_WIDTH, WIDGET_HEIGHT);
}

}  // namespace dev
