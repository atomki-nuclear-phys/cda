// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_DT5740_QTHIST_H
#define CDA_DEVICES_DT5740_QTHIST_H

// System include(s):
#include <memory>

// Qt include(s):
#include <QtCore/QtGlobal>
#include <QStackedLayout>
#include <QTabWidget>
#include <QVBoxLayout>

// CDA include(s):
#include "device/QtHist.h"
#include "caen/Digitizer.h"
#include "msg/Logger.h"
#include "moni/Histogram.h"

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
      /// Destructor
      ~QtHist();

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
      /// The possibly created raw histograms
      std::unique_ptr< RawHistogram >
      m_rawHistograms[ NUMBER_OF_GROUPS ][ GroupConfig::CHANNELS_IN_GROUP ];
      /// The possibly created reconstructed histograms
      std::unique_ptr< moni::Histogram >
      m_histograms[ NUMBER_OF_GROUPS ][ GroupConfig::CHANNELS_IN_GROUP ][ 2 ];
      /// Widgets for the tabs
      std::unique_ptr< QWidget >
      m_widgets[ NUMBER_OF_GROUPS ][ GroupConfig::CHANNELS_IN_GROUP ];
      /// Layouts for the tabs
      std::unique_ptr< QVBoxLayout >
      m_layouts[ NUMBER_OF_GROUPS ][ GroupConfig::CHANNELS_IN_GROUP ];

      mutable caen::Digitizer::EventInfo m_eventInfo; ///< Decoded event info
      mutable caen::Digitizer::EventData16Bit m_eventData; ///< Decoded event data

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class QtHist

} // namespace dt5740

#endif // CDA_DEVICES_DT5740_QTHIST_H
