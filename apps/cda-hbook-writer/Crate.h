// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_APPS_CDA_HBOOK_WRITER_CRATE_H
#define CDA_APPS_CDA_HBOOK_WRITER_CRATE_H

// Qt include(s):
#include <QtCore/QtGlobal>
#include <QtCore/QString>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/Crate.h"
#   include "cdacore/device/Disk.h"
#   include "cdacore/cernlib/NTupleMgr.h"
#   include "cdacore/event/Event.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "device/Crate.h"
#   include "device/Disk.h"
#   include "cernlib/NTupleMgr.h"
#   include "event/Event.h"
#   include "msg/Logger.h"
#endif

namespace hbook {

   class Crate : public dev::Crate< dev::Disk > {

   public:
      Crate();
      ~Crate();

      bool initialize( const QString& fileName );
      bool writeEvent( const ev::Event& event );
      bool finalize();

   private:
      cernlib::NTupleMgr m_nmgr;
      mutable msg::Logger m_logger;

   }; // class Crate

} // namespace hbook

#endif // CDA_APPS_CDA_HBOOK_WRITER_CRATE_H
