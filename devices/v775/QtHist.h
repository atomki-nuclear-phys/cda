// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_V775_QTHIST_H
#define CDA_DEVICES_V775_QTHIST_H

// System include(s):
#include <memory>
#include <array>

// Qt include(s):
#include <QStackedLayout>
#include <QTabWidget>

// CDA include(s):
#include "device/QtHist.h"
#include "msg/Logger.h"
#include "moni/Histogram.h"

// Local include(s):
#include "Device.h"

namespace v775 {

   /// Class providing Qt-only monitoring for a V775 device
   ///
   /// This class is used to monitor the data taking of V775 devices in a way
   /// that doesn't rely on any external library.
   ///
   /// @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
   ///
   /// $Revision$
   /// $Date$
   ///
   class QtHist : public dev::QtHist,
                  public virtual Device {

      Q_OBJECT

   public:
      /// Regular Qt constructor
      QtHist( QWidget* parent = 0, Qt::WindowFlags flags = Qt::WindowFlags() );
      /// Destructor
      ~QtHist();

      /// @name Function(s) reimplemented from v775::Device
      /// @{

      /// Read the device configuration from a binary file
      virtual StatusCode readConfig( QIODevice& dev );
      /// Read the device configuration from an XML file
      virtual StatusCode readConfig( const QDomElement& node );

      /// @}

      /// @name Function(s) inherited from dev::QtHist
      /// @{

      /// Function displaying a newly received event
      virtual bool displayEvent( const ev::Fragment& fragment );
      /// Title to show for the window displaying this object
      virtual QString windowTitle() const;

      /// @}

   private:
      /// Initialize the device after reading a configuration
      bool initialize();

      /// Layout for the tab widget
      std::unique_ptr< QStackedLayout > m_channelLayout;
      /// Separate tabs for the channels
      std::unique_ptr< QTabWidget > m_channelTab;

      /// Histograms for the monitored channels
      std::array< std::unique_ptr< moni::Histogram >,
                  NUMBER_OF_CHANNELS > m_histograms;

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class QtHist

} // namespace v775

#endif // CDA_DEVICES_V775_QTHIST_H
