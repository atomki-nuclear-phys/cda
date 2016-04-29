// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_CAEN_VMEDEVICE_H
#define CDA_CORE_CAEN_VMEDEVICE_H

// System include(s):
#include <cinttypes>
#include <vector>

// Qt include(s):
#include <QtCore/QCoreApplication>

// Local include(s):
#include "../common/Export.h"
#include "../msg/Logger.h"

namespace caen {

   // Forward declaration(s):
   class VmeBus;
   class VmeDevicePrivateData;

   /**
    * @brief Interface to the CAENqtpSDK library
    *
    * This class is used to insulate the device code from the CAENqtpSDK
    * library, which is used to communicate with all the CAEN VME devices used
    * by the project. It pretty much just provides a C++ interface to the
    * library, providing some simulated data for testing purposes when the code
    * is built without that library.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class CDACORE_EXPORT VmeDevice {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( caen::VmeDevice )

   public:
      /// Default constructor
      VmeDevice();
      /// Destructor
      ~VmeDevice();

      /// Supported device types
      enum DeviceType {
         DEV_V775A = 0, ///< The board is a V775A
         DEV_V785A = 1, ///< The board is a V785A
         DEV_V792A = 2, ///< The board is a V792A
         DEV_V862  = 3, ///< The board is a V862
         DEV_UNKNOWN = 99 ///< Default value for an unknown device
      };

      /// @name Functions used in connecting to the device
      /// @{

      /// Open connection to a CAEN VME device
      bool open( uint16_t address, DeviceType type, const VmeBus& bus );
      /// Close the connection to the device
      bool close();

      /// Print some information about the board
      bool printInfo( msg::Level level );

      /// Clear the board of any data. Usually called during initialisation.
      bool dataClear();

      /// @}

      /// @name Functions setting board parameters
      /// @{

      /// Set up the zero suppression of the device
      bool setZeroSuppression( bool enable, bool stepThreshold,
                               const std::vector< uint16_t >& thresholds );

      /// Set up the acquisition mode of the device
      bool setAcquisitionMode( bool slidingScaleEnable,
                               bool zeroSuppressionEnable,
                               bool overflowSuppressionEnable,
                               bool validSuppressionEnable,
                               bool commonStopEnable,
                               bool emptyEnable,
                               bool countAllEvents );

      /// Set the readout mode of the device
      bool setReadoutMode( bool busErrorEnable, bool blockEndEnable,
                           bool align64Enable );

      /// Set the pedestal of the device
      bool setPedestal( uint8_t value );

      /// @}

      /// @name Functions/types used in the data acquisition
      /// @{

      /// Struct describing the header of an event read from the device
      struct DataHeader {
         uint32_t geo;
         uint32_t crate;
         uint32_t channelCount;
      };
      /// Struct describing the end-of-event word
      struct DataFooter {
         uint32_t geo;
         uint32_t eventCount;
      };
      /// Struct describing a single data word read from the device
      struct DataWord {
         uint32_t geo;
         uint32_t channel;
         uint32_t data;
         uint32_t underThreshold;
         uint32_t overflow;
      };
      /// Struct describing a single event read from the device
      struct DataEvent {
         DataHeader header;
         DataFooter footer;
         std::vector< DataWord > data;
      };

      /// Read data from the board, and fill the user provided data object
      bool read( std::vector< DataEvent >& events );

      /// @}

   private:
      /// Function making sure that the data object is pristene clear
      void resetData();
      /// Function checking that the device is connected to
      bool isConnected() const;

      /// Internal data for the device
      VmeDevicePrivateData* m_data;
      /// The device type
      DeviceType m_type;

      /// The message logger object
      msg::Logger m_logger;

   }; // class VmeDevice

} // namespace caen

#endif // CDA_CORE_CAEN_VMEDEVICE_H
