// $Id$

// Local include(s):
#include "ClearAction.h"

namespace dev {

   /**
    * The constructor makes sure that when this action is triggered by
    * the underlying QAction object, it will also emit its "extended"
    * triggered(int) signal.
    *
    * @param text The displayed text of the action
    * @param slot The crate slot that should be cleared
    * @param parent The Qt parent of the object
    */
   ClearAction::ClearAction( const QString& text, int slot,
                             QObject* parent )
      : QAction( text, parent ), m_slot( slot ) {

      connect( this, SIGNAL( triggered() ),
               this, SLOT( triggeredSlot() ) );

   }

   /**
    * When this slot is called by the triggered() signal of the base class,
    * it emits the triggered(int) signal.
    */
   void ClearAction::triggeredSlot() {

      emit triggered( m_slot );
      return;

   }

} // namespace dev
