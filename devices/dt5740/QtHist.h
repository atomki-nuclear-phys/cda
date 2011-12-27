// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_DT5740_QTHIST_H
#define CDA_DEVICES_DT5740_QTHIST_H

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/QtHist.h"
#   include "cdacore/caen/Digitizer.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "device/QtHist.h"
#   include "caen/Digitizer.h"
#   include "msg/Logger.h"
#endif

// Local include(s):
#include "Device.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QStackedLayout )
QT_FORWARD_DECLARE_CLASS( QScrollArea )
QT_FORWARD_DECLARE_CLASS( QTabWidget )
QT_FORWARD_DECLARE_CLASS( QVBoxLayout )
namespace moni {
   class Histogram;
}

namespace dt5740 {

   // Forward declaration(s):
   class RawHistogram;

   class QtHist : public dev::QtHist,
                  public virtual Device {

      Q_OBJECT

   public:
      /// Qt Widget constructor
      QtHist( QWidget* parent = 0, Qt::WindowFlags flags = 0 );
      /// Destructor
      ~QtHist();

      /// Read the device configuration from a binary file
      virtual bool readConfig( QIODevice* dev );
      /// Read the device configuration from an XML file
      virtual bool readConfig( const QDomElement& node );

      /// Function displaying a newly received event
      virtual bool displayEvent( const ev::Fragment& fragment );

   private:
      /// Initialize the device after reading a configuration
      bool initialize();
      /// Reset the device
      bool reset();

      QStackedLayout* m_channelLayout; ///< Layout for the tab widget
      QTabWidget* m_channelTab; ///< Separate tabs for the channels
      /// The possibly created raw histograms
      RawHistogram*
      m_rawHistograms[ NUMBER_OF_GROUPS ][ GroupConfig::CHANNELS_IN_GROUP ];
      /// The possibly created reconstructed histograms
      moni::Histogram*
      m_histograms[ NUMBER_OF_GROUPS ][ GroupConfig::CHANNELS_IN_GROUP ][ 2 ];
      /// Widgets for the tabs
      QWidget*
      m_widgets[ NUMBER_OF_GROUPS ][ GroupConfig::CHANNELS_IN_GROUP ];
      /// Layouts for the tabs
      QVBoxLayout*
      m_layouts[ NUMBER_OF_GROUPS ][ GroupConfig::CHANNELS_IN_GROUP ];

      mutable caen::Digitizer::EventInfo m_eventInfo; ///< Decoded event info
      mutable caen::Digitizer::EventData16Bit m_eventData; ///< Decoded event data

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class QtHist

} // namespace dt5740

#endif // CDA_DEVICES_DT5740_QTHIST_H
