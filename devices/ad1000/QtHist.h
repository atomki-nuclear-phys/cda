// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_AD1000_QTHIST_H
#define CDA_DEVICES_AD1000_QTHIST_H

// System include(s):
#include <memory>

// Qt include(s):
#include <QStackedLayout>
#include <QtCore/QtGlobal>

// CDA include(s):
#include "device/QtHist.h"
#include "moni/Histogram.h"
#include "msg/Logger.h"

// Local include(s):
#include "Device.h"

namespace ad1000 {

/**
 *  @short Qt-only monitoring for an AD1000 device
 *
 *         This class provides a completely standalone way of monitoring
 *         the data collected by an AD1000 CAMAC device. It simply just shows
 *         a single histogram with the data collected by the device on its
 *         only input channel.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 *
 * $Revision$
 * $Date$
 */
class QtHist : public dev::QtHist, public virtual Device {

   Q_OBJECT

public:
   /// Qt Widget constructor
   QtHist(QWidget* parent = 0, Qt::WindowFlags flags = Qt::WindowFlags());

   /// Read the device configuration from a binary file
   virtual StatusCode readConfig(QIODevice& dev);
   /// Read the device configuration from an XML file
   virtual StatusCode readConfig(const QDomElement& node);

   /// Function displaying a newly received event
   virtual bool displayEvent(const ev::Fragment& fragment);

private:
   /// Initialize the device after reading a configuration
   bool initialize();

   /// Layout needed for the histogram
   std::unique_ptr<QStackedLayout> m_layout;
   /// Histogram of the one available variable
   std::unique_ptr<moni::Histogram> m_hist;

   mutable msg::Logger m_logger;  ///< Message logger object

};  // class QtHist

}  // namespace ad1000

#endif  // CDA_DEVICES_AD1000_QTHIST_H
