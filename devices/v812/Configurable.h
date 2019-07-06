// Dear emacs, this is -*- c++ -*-
#ifndef CDA_DEVICES_V812_CONFIGURABLE_H
#define CDA_DEVICES_V812_CONFIGURABLE_H

// Local include(s).
#include "Device.h"

// CDA include(s).
#include "device/ICaenVmeConfigurable.h"
#include "msg/Logger.h"

namespace v812 {

   /// Class performing the configuration of a V812 CFD
   ///
   /// This is the class that, based on an XML/binary configuration can
   /// configure a CAEN V812 VME CFD through its VME interface.
   ///
   /// @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
   ///
   class Configurable : public Device,
                        public virtual dev::ICaenVmeConfigurable {

   public:
      /// Default constructor
      Configurable() = default;

      /// @name Function(s) inherited from @c dev::CaenVmeConfigurable
      /// @{

      /// Configure the device for data taking
      virtual StatusCode configure( const caen::VmeBus& bus ) const override;

      /// @}

   private:
      /// Message logger object
      mutable msg::Logger m_logger{ "v812::Configurable" };

   }; // class Configurable

} // namespace v812

#endif // CDA_DEVICES_V812_CONFIGURABLE_H
