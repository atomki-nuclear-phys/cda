
// Local include(s):
#include "CreateAction.h"

namespace dev {

/**
 * The constructor makes sure that when this action is triggered by
 * the underlying QAction object, it will also emit its "extended"
 * triggered(int,QString) signal.
 *
 * @param text The displayed text of the action
 * @param slot The crate slot in which the device should be created
 * @param type The type of the module that should be created
 * @param parent The Qt parent of the object
 */
CreateAction::CreateAction(const QString& text, int slot, const QString& type,
                           QObject* parent)
    : QAction(text, parent), m_slot(slot), m_type(type) {

   connect(this, SIGNAL(triggered()), this, SLOT(triggeredSlot()));
}

/**
 * When this slot is called by the triggered() signal of the base class,
 * it emits the triggered(int,QString) signal.
 */
void CreateAction::triggeredSlot() {

   emit triggered(m_slot, m_type);
   return;
}

}  // namespace dev
