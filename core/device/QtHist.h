// Dear emacs, this is -*- c++ -*-
#ifndef CDA_CORE_DEVICE_QTHIST_H
#define CDA_CORE_DEVICE_QTHIST_H

// Qt include(s):
#include <QWidget>

// Local include(s):
#include "../common/Export.h"
#include "IDevice.h"

// Forward declaration(s):
namespace ev {
class Fragment;
}

namespace dev {

//
// Make sure that the following Qt classes are available in the
// DEV namespace even if Qt has been built in an arbitrary
// namespace:
//
using QT_PREPEND_NAMESPACE(QWidget);

/**
 *  @short Base class for a device monitoring class
 *
 *         In order to be able to monitor the data acquisition even
 *         without CERNLIB, the devices can produce some monitoring
 *         histograms "from scratch", just using the Qt+CDA code.
 *
 *         Notice that unlike for most graphical classes, there are
 *         no size limitations for this class.
 *
 *         This is the base class for such monitoring classes.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */
class CDACORE_EXPORT QtHist : public QWidget, virtual public IDevice {

   Q_OBJECT

public:
   /// Standard QWidget style constructor
   QtHist(QWidget* parent = 0, Qt::WindowFlags flags = Qt::WindowFlags());

   /// Function displaying a newly received event
   virtual bool displayEvent(const ev::Fragment& fragment) = 0;

   /// Title to show for the window displaying this object
   virtual QString windowTitle() const;

};  // class QtHist

}  // namespace dev

#endif  // CDA_CORE_DEVICE_QTHIST_H
