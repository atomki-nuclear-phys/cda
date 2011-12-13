// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_DT5740_GUI_H
#define CDA_DEVICES_DT5740_GUI_H

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/CaenGui.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "device/CaenGui.h"
#   include "msg/Logger.h"
#endif

// Local include(s):
#include "Device.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QLabel )
QT_FORWARD_DECLARE_CLASS( QCheckBox )
QT_FORWARD_DECLARE_CLASS( QScrollArea )
QT_FORWARD_DECLARE_CLASS( QGroupBox )
QT_FORWARD_DECLARE_CLASS( QComboBox )
QT_FORWARD_DECLARE_CLASS( QSpinBox )

namespace dt5740 {

   // Forward declaration(s):
   class GroupGui;

   //
   // Make sure that the following Qt classes are available in the
   // DEV namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QLabel );
   using QT_PREPEND_NAMESPACE( QCheckBox );
   using QT_PREPEND_NAMESPACE( QScrollArea );

   /**
    *  @short Graphical repsesentation of the DT5640 digitizer
    *
    *         This widget is used to change the settings of a
    *         CAEN DT5740 digitizer.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class Gui : public dev::CaenGui,
               public virtual Device {

      Q_OBJECT

   public:
      /// Constructor
      Gui( QWidget* parent = 0, Qt::WindowFlags flags = 0 );
      /// Destructor
      ~Gui();

      /// Read the device configuration from a binary file
      virtual bool readConfig( QIODevice* dev );
      /// Read the device configuration from an XML file
      virtual bool readConfig( const QDomElement& node );

   private slots:
      /// Slot handling changes to the connection mode
      void connectionModeSlot( int index );
      /// Slot handling changes to the connection link
      void connectionLinkSlot( int link );

   private:
      /// Function "syncing" the configuration
      void sync();

      QScrollArea* m_scrollArea; ///< Object providing the scrolling functions
      QWidget*     m_scrollWidget; ///< Main widget that is scrolled

      QLabel*     m_image; ///< Image of the digitizer itself
      QLabel*     m_topLabel; ///< Text label at the top

      QGroupBox* m_connectionBox; ///< Box for the connection parameters
      QLabel*    m_connModeLabel; ///< Description for conn. mode setting
      QComboBox* m_connMode;      ///< Connection mode chooser
      QLabel*    m_connLinkLabel; ///< Description for conn. link setting
      QSpinBox*  m_connLink;      ///< Connection link chooser

      /// The graphical representation of channel groups
      GroupGui* m_ggroups[ NUMBER_OF_GROUPS ];

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class Gui

} // namespace dt5740

#endif // CDA_DEVICES_DT5740_GUI_H
