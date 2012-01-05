// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_AD1000_QTHIST_H
#define CDA_DEVICES_AD1000_QTHIST_H

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/QtHist.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "device/QtHist.h"
#   include "msg/Logger.h"
#endif

// Local include(s):
#include "Device.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QStackedLayout )
namespace moni {
   class Histogram;
}

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

      QStackedLayout* m_layout; ///< Layout needed for the histogram
      moni::Histogram* m_hist; ///< Histogram of the one available variable

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class QtHist

} // namespace ad1000

#endif // CDA_DEVICES_AD1000_QTHIST_H
