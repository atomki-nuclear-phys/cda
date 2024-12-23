// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_AD2249A_ROOTDISK_H
#define CDA_DEVICES_AD2249A_ROOTDISK_H

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#include "device/IRootDisk.h"
#include "msg/Logger.h"

// Local include(s):
#include "Device.h"

namespace ad2249a {

/**
 *  @short ROOT file writer for an AD2249A CAMAC device
 *
 *         This class can write out the pulse heights converted by
 *         an AD2249A CAMAC device into a ROOT file.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 *
 * $Revision$
 * $Date$
 */
class RootDisk : public virtual dev::IRootDisk, public virtual Device {

   // To get the tr() function:
   Q_DECLARE_TR_FUNCTIONS(ad2249a::RootDisk)

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

}  // namespace ad2249a

#endif  // CDA_DEVICES_AD2249A_ROOTDISK_H
