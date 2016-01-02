// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_DT5740_QTHIST_H
#define CDA_DEVICES_DT5740_QTHIST_H

// Qt include(s):
#include <QtCore/QtGlobal>
#include <QStackedLayout>
#include <QTabWidget>
#include <QVBoxLayout>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/QtHist.h"
#   include "cdacore/caen/Digitizer.h"
#   include "cdacore/msg/Logger.h"
#   include "cdacore/common/UniquePtr.h"
#   include "cdadaq/moni/Histogram.h"
#else
#   include "device/QtHist.h"
#   include "caen/Digitizer.h"
#   include "msg/Logger.h"
#   include "common/UniquePtr.h"
#   include "moni/Histogram.h"
#endif

// Local include(s):
#include "Device.h"
#include "RawHistogram.h"

namespace dt5740 {

   /**
    *  @short Qt-only monitoring for a DT5740 device
    *
    *         This class provides a completely standalone way of monitoring
    *         the data collected by a DT5740 device. It uses the custom
    *         histogram code to display both the daw data that's being
    *         collected, and the distribution of the reconstructed
    *         variables.
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
      /// The possibly created raw histograms
      UniquePtr< RawHistogram >::Type
      m_rawHistograms[ NUMBER_OF_GROUPS ][ GroupConfig::CHANNELS_IN_GROUP ];
      /// The possibly created reconstructed histograms
      UniquePtr< moni::Histogram >::Type
      m_histograms[ NUMBER_OF_GROUPS ][ GroupConfig::CHANNELS_IN_GROUP ][ 2 ];
      /// Widgets for the tabs
      UniquePtr< QWidget >::Type
      m_widgets[ NUMBER_OF_GROUPS ][ GroupConfig::CHANNELS_IN_GROUP ];
      /// Layouts for the tabs
      UniquePtr< QVBoxLayout >::Type
      m_layouts[ NUMBER_OF_GROUPS ][ GroupConfig::CHANNELS_IN_GROUP ];

      mutable caen::Digitizer::EventInfo m_eventInfo; ///< Decoded event info
      mutable caen::Digitizer::EventData16Bit m_eventData; ///< Decoded event data

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class QtHist

} // namespace dt5740

#endif // CDA_DEVICES_DT5740_QTHIST_H
