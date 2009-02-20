// Dear emacs, this is -*- c++ -*-
// $Id: Gui.h 32 2008-07-10 14:15:52Z krasznaa $
#ifndef CDA_DEVICES_AD2249A_GUI_H
#define CDA_DEVICES_AD2249A_GUI_H

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/Gui.h"
#else
#   include "device/Gui.h"
#endif

// Local include(s):
#include "Device.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QLabel )
QT_FORWARD_DECLARE_CLASS( QCheckBox )
QT_FORWARD_DECLARE_CLASS( QScrollArea )

namespace ad2249a {

   // Forward declaration(s):
   class ChannelGui;

   //
   // Make sure that the following Qt classes are available in the
   // DEV namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QLabel );
   using QT_PREPEND_NAMESPACE( QCheckBox );
   using QT_PREPEND_NAMESPACE( QScrollArea );

   /**
    *  @short Graphical repsesentation of the LeCroy 2249A
    *
    *         This widget is used to change the settings of a LeCroy
    *         2248A 12 channel Analog-to-Digital Converter. It adheres to the
    *         rules described in the dev::Gui and dev::CrateWidget
    *         classes.
    *
    *         The configuration widget of the device looks something like
    *         this on a Mac:
    *
    *         @image html ad2249a_Gui.png
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision: 32 $
    * $Date: 2008-07-10 16:15:52 +0200 (Thu, 10 Jul 2008) $
    */
   class Gui : public virtual dev::Gui,
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

      /// Draw a graphical representation of the module
      virtual void drawModule( QPainter& painter ) const;

   private slots:
      /// Slot handling changes to channel enablement
      void channelEnabledSlot( int subaddress, bool on );
      /// Slot handling changes to channel names
      void nameChangedSlot( int subaddress, const QString& text );
      /// Slot handling changes to histogram channel numbers
      void channelsChangedSlot( int subaddress, int channels );
      /// Slot handling changes to histogram lower bounds
      void lowerBoundChangedSlot( int subaddress, double value );
      /// Slot handling changes to histogram upper bounds
      void upperBoundChangedSlot( int subaddress, double value );
      /// Slot handling changes to the LAM setting
      void generateLamChangedSlot( bool on );

   private:
      /// Function "syncing" the configuration
      void sync();

      QScrollArea* m_scrollArea; ///< Object providing the scrolling functions
      QWidget*     m_scrollWidget; ///< Main widget that is scrolled

      QLabel*     m_topLabel; ///< Text label at the top

      QLabel*     m_nameLabel; ///< Label above channel names
      QLabel*     m_channelsLabel; ///< Label above histogram channels
      QLabel*     m_lowerBoundLabel; ///< Label above histogram lower bounds
      QLabel*     m_upperBoundLabel; ///< Label above histogram upper bounds

      /// The graphical channel representations
      ChannelGui* m_gchannels[ NUMBER_OF_SUBADDRESSES ];

      QCheckBox*  m_generateLamEdit; ///< Widget for changing the LAM setting

   }; // class Gui

} // namespace ad2249a

#endif // CDA_DEVICES_AD2249A_GUI_H
