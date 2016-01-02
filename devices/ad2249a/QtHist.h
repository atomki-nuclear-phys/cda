// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_DT2249A_QTHIST_H
#define CDA_DEVICES_DT2249A_QTHIST_H

// Qt include(s):
#include <QtCore/QtGlobal>
#include <QStackedLayout>
#include <QTabWidget>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/QtHist.h"
#   include "cdacore/msg/Logger.h"
#   include "cdacore/common/UniquePtr.h"
#   include "cdadaq/moni/Histogram.h"
#else
#   include "device/QtHist.h"
#   include "msg/Logger.h"
#   include "common/UniquePtr.h"
#   include "moni/Histogram.h"
#endif

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
      QtHist( QWidget* parent = 0, Qt::WindowFlags flags = 0 );

      /// Read the device configuration from a binary file
      virtual bool readConfig( QIODevice& dev );
      /// Read the device configuration from an XML file
      virtual bool readConfig( const QDomElement& node );

      /// Function displaying a newly received event
      virtual bool displayEvent( const ev::Fragment& fragment );

   private:
      /// Initialize the device after reading a configuration
      bool initialize();
      /// Reset the device
      bool reset();

      /// Layout for the tab widget
      UniquePtr< QStackedLayout >::Type m_channelLayout;
      /// Separate tabs for the channels
      UniquePtr< QTabWidget >::Type m_channelTab;
      /// The histograms
      UniquePtr< moni::Histogram >::Type m_histograms[ NUMBER_OF_SUBADDRESSES ];

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class QtHist

} // namespace ad2249a

#endif // CDA_DEVICES_DT2249A_QTHIST_H
