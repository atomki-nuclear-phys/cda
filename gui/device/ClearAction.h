// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_GUI_DEVICE_CLEARACTION_H
#define CDA_GUI_DEVICE_CLEARACTION_H

// Qt include(s):
#include <QAction>

namespace dev {

   /**
    *  @short Action requesting the clearing of a CAMAC slot
    *
    *         I need specialised action objects in CrateWidget to handle
    *         the context-menus. This class is used to signal to the
    *         CrateWidget object that one of its slots should be cleared.
    *
    *         The class is a very thin layer around QAction, just adding
    *         a slot variable to its triggered() signal.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class ClearAction : public QAction {

      Q_OBJECT

   public:
      /// Constructor
      ClearAction( const QString& text, int slot,
                   QObject* parent = 0 );

   signals:
      /// Signal emitted when the action is called
      /**
       * @param slot The crate slot that should be cleared
       */
      void triggered( int slot );

   private slots:
      /// Internal slot for emitting the triggered() signal
      void triggeredSlot();

   private:
      int m_slot; ///< The crate slot that should be cleared

   }; // class ClearAction

} // namespace dev

#endif // CDA_GUI_DEVICE_CLEARACTION_H
