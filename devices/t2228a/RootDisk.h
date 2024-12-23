// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_T2228A_ROOTDISK_H
#define CDA_DEVICES_T2228A_ROOTDISK_H

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#include "device/IRootDisk.h"
#include "msg/Logger.h"

// Local include(s):
#include "Device.h"

namespace t2228a {

/**
 *  @short ROOT file writer for a T2228A CAMAC device
 *
 *         This class can write out the time differences converted by
 *         a T2228A CAMAC device into a ROOT file.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 *
 * $Revision$
 * $Date$
 */
class RootDisk : public virtual dev::IRootDisk, public virtual Device {

   // To get the tr() function:
   Q_DECLARE_TR_FUNCTIONS(t2228a::RootDisk)

public:
   /// Constructor
   RootDisk();

   /// Function initializing the device
   virtual bool initialize(root::NTupleMgr& nmgr);

   /// Function filling the ntuple
   virtual bool writeEvent(const ev::Fragment& fragment) const;

private:
   /// The values read from the device
   mutable unsigned int m_values[NUMBER_OF_SUBADDRESSES];

   mutable msg::Logger m_logger;  ///< Message logger object

};  // class RootDisk

}  // namespace t2228a

#endif  // CDA_DEVICES_T2228A_ROOTDISK_H
