// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_AD1000_GUI_H
#define CDA_DEVICES_AD1000_GUI_H

// System include(s):
#include <memory>

// Qt include(s):
#include <QCheckBox>
#include <QLabel>
#include <QtCore/QtGlobal>

// CDA include(s):
#include "device/CamacGui.h"
#include "msg/Logger.h"

// Local include(s):
#include "ChannelGui.h"
#include "Device.h"

namespace ad1000 {

// Bring the Qt classes into this namespace:
using QT_PREPEND_NAMESPACE(QLabel);
using QT_PREPEND_NAMESPACE(QCheckBox);

/**
 *  @short Graphical repsesentation of the AD1000 ADC
 *
 *         This widget is used to change the settings of an Atomki
 *         AD1000 ADC.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 *
 * $Revision$
 * $Date$
 */
class Gui : public dev::CamacGui, public virtual Device {

   Q_OBJECT

public:
   /// Constructor
   Gui(QWidget* parent = 0, Qt::WindowFlags flags = Qt::WindowFlags());

   /// Read the device configuration from a binary file
   virtual StatusCode readConfig(QIODevice& dev);
   /// Read the device configuration from an XML file
   virtual StatusCode readConfig(const QDomElement& node);

   /// Draw a graphical representation of the module
   virtual void drawModule(QPainter& painter) const;
   /// Width of the device in units of crate slots
   virtual int deviceWidth() const;

private slots:
   /// Slot handling changes to channel names
   void nameChangedSlot(const QString& text);
   /// Slot handling changes to histogram channel numbers
   void channelsChangedSlot(int channels);
   /// Slot handling changes to histogram lower bounds
   void lowerBoundChangedSlot(double value);
   /// Slot handling changes to histogram upper bounds
   void upperBoundChangedSlot(double value);
   /// Slot handling changes to the LAM setting
   void generateLamChangedSlot(bool on);

private:
   /// Function "syncing" the configuration
   void sync();

   /// Text label at the top
   std::unique_ptr<QLabel> m_topLabel;

   /// Label above channel names
   std::unique_ptr<QLabel> m_nameLabel;
   /// Label above histogram channels
   std::unique_ptr<QLabel> m_channelsLabel;
   /// Label above histogram lower bounds
   std::unique_ptr<QLabel> m_lowerBoundLabel;
   /// Label above histogram upper bounds
   std::unique_ptr<QLabel> m_upperBoundLabel;

   /// The graphical channel representation
   std::unique_ptr<ChannelGui> m_gchannel;

   /// Widget for changing the LAM setting
   std::unique_ptr<QCheckBox> m_generateLamEdit;

   mutable msg::Logger m_logger;  ///< Message logger object

};  // class Gui

}  // namespace ad1000

#endif  // CDA_DEVICES_AD1000_GUI_H
