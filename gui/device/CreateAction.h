// Dear emacs, this is -*- c++ -*-
#ifndef CDA_GUI_DEVICE_CREATEACTION_H
#define CDA_GUI_DEVICE_CREATEACTION_H

// Qt include(s):
#include <QString>
#include <QAction>

namespace dev {

   /**
    *  @short Action requesting the creation of a CAMAC module
    *
    *         I need specialised action objects in CrateWidget to handle
    *         the context-menus. This class is used to signal to the
    *         CrateWidget object that a new device should be created in
    *         one of its slots.
    *
    *         The class is a very thin layer around QAction, just adding
    *         a slot and type variable to its triggered() signal.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    */
   class CreateAction : public QAction {

      Q_OBJECT

   public:
      /// Constructor
      CreateAction( const QString& text, int slot,
                    const QString& type, QObject* parent = 0 );

   signals:
      /// Signal emitted when the action is called
      /**
       * @param slot The crate slot in which the device should be created
       * @param type The type of the module that should be created
       */
      void triggered( int slot, const QString& type );

   private slots:
      /// Internal slot for emitting the triggered() signal
      void triggeredSlot();

   private:
      int     m_slot; ///< The crate slot in which the device should be created
      QString m_type; ///< The type of the module that should be created

   }; // class CreateAction

} // namespace dev

#endif // CDA_GUI_DEVICE_CREATEACTION_H
