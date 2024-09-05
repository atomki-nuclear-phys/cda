// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_DT2249A_QTHIST_H
#define CDA_DEVICES_DT2249A_QTHIST_H

// System include(s):
#include <memory>

// Qt include(s):
#include <QtCore/QtGlobal>
#include <QStackedLayout>
#include <QTabWidget>

// CDA include(s):
#include "device/QtHist.h"
#include "msg/Logger.h"
#include "moni/Histogram.h"

// Local include(s):
#include "Device.h"

namespace ad2249a {

   /**
    *  @short Qt-only monitoring for an AD2249A device
    *
    *         This class provides a completely standalone way of monitoring
    *         the data collected by an AD2249A CAMAC device. It shows the data
    *         collected by all the enabled channels in a separate histogram.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class QtHist : public dev::QtHist,
                  public virtual Device {

      Q_OBJECT

   public:
      /// Qt Widget constructor
      QtHist( QWidget* parent = 0, Qt::WindowFlags flags = Qt::WindowFlags() );

      /// Read the device configuration from a binary file
      virtual StatusCode readConfig( QIODevice& dev );
      /// Read the device configuration from an XML file
      virtual StatusCode readConfig( const QDomElement& node );

      /// Function displaying a newly received event
      virtual bool displayEvent( const ev::Fragment& fragment );

   private:
      /// Initialize the device after reading a configuration
      bool initialize();
      /// Reset the device
      bool reset();

      /// Layout for the tab widget
      std::unique_ptr< QStackedLayout > m_channelLayout;
      /// Separate tabs for the channels
      std::unique_ptr< QTabWidget > m_channelTab;
      /// The histograms
      std::unique_ptr< moni::Histogram > m_histograms[ NUMBER_OF_SUBADDRESSES ];

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class QtHist

} // namespace ad2249a

#endif // CDA_DEVICES_DT2249A_QTHIST_H
