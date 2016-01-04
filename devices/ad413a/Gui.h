// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_AD413A_GUI_H
#define CDA_DEVICES_AD413A_GUI_H

// System include(s):
#include <memory>

// Qt include(s):
#include <QtCore/QtGlobal>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>
#include <QScrollArea>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/CamacGui.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "device/CamacGui.h"
#   include "msg/Logger.h"
#endif

// Local include(s):
#include "Device.h"
#include "ChannelGui.h"

namespace ad413a {

   //
   // Make sure that the following Qt classes are available in the
   // DEV namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QLabel );
   using QT_PREPEND_NAMESPACE( QCheckBox );
   using QT_PREPEND_NAMESPACE( QSpinBox );
   using QT_PREPEND_NAMESPACE( QScrollArea );

   /**
    *  @short Graphical repsesentation of the ORTEC AD413A
    *
    *         This widget is used to change the settings of an ORTEC
    *         AD413A 4 channel Analog-to-Digital Converter. It adheres to the
    *         rules described in the dev::Gui and dev::CrateWidget
    *         classes.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class Gui : public dev::CamacGui,
               public virtual Device {

      Q_OBJECT

   public:
      /// Constructor
      Gui( QWidget* parent = 0, Qt::WindowFlags flags = 0 );

      /// Read the device configuration from a binary file
      virtual bool readConfig( QIODevice& dev );
      /// Read the device configuration from an XML file
      virtual bool readConfig( const QDomElement& node );

      /// Draw a graphical representation of the module
      virtual void drawModule( QPainter& painter ) const;
      /// Width of the device in units of crate slots
      virtual int deviceWidth() const;

   private slots:
      /// Slot handling changes to channel enablement
      void channelEnabledSlot( int subaddress, bool on );
      /// Slot handling changes to channel names
      void nameChangedSlot( int subaddress, const QString& text );
      /// Slot handling changes to low level discriminants
      void lldChangedSlot( int subaddress, int lld );
      /// Slot handling changes to histogram channel numbers
      void channelsChangedSlot( int subaddress, int channels );
      /// Slot handling changes to histogram lower bounds
      void lowerBoundChangedSlot( int subaddress, double value );
      /// Slot handling changes to histogram upper bounds
      void upperBoundChangedSlot( int subaddress, double value );
      /// Slot handling changes to the LAM setting
      void generateLamChangedSlot( bool on );
      /// Slot handling changes in the gate setting
      void gateChangedSlot( int gate );

   private:
      /// Function "syncing" the configuration
      void sync();

      /// Object providing the scrolling functions
      std::unique_ptr< QScrollArea > m_scrollArea;
      /// Main widget that is scrolled
      std::unique_ptr< QWidget >     m_scrollWidget;

      /// Text label at the top
      std::unique_ptr< QLabel > m_topLabel;

      /// Label above channel names
      std::unique_ptr< QLabel > m_nameLabel;
      /// Label above low level discriminants
      std::unique_ptr< QLabel > m_lldLabel;
      /// Label above histogram channels
      std::unique_ptr< QLabel > m_channelsLabel;
      /// Label above histogram lower bounds
      std::unique_ptr< QLabel > m_lowerBoundLabel;
      /// Label above histogram upper bounds
      std::unique_ptr< QLabel > m_upperBoundLabel;

      /// The graphical channel representations
      std::unique_ptr< ChannelGui > m_gchannels[ NUMBER_OF_SUBADDRESSES ];

      /// Widget for changing the LAM setting
      std::unique_ptr< QCheckBox > m_generateLamEdit;
      /// Label for the gate spinbox
      std::unique_ptr< QLabel > m_gateLabel;
      /// Widget for changing the gate setting
      std::unique_ptr< QSpinBox > m_gateEdit;

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class Gui

} // namespace ad413a

#endif // CDA_DEVICES_AD413A_GUI_H
