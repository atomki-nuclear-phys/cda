// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_CAEN_STOPACQUISITION_H
#define CDA_CORE_CAEN_STOPACQUISITION_H

/// Flag specifying whether the event taking should continue
/**
 * The CAEN libraries don't like it when we exit from one of them using
 * a keyboard interrupt. I guess that in this case some hidden structures
 * are left in an inconsistent state...
 *
 * To solve this, the CAEN library using plugins have to keep an eye on
 * this variable to devise whether they should continue waiting for an
 * incoming event. If the variable is set to <code>true</code>, the device
 * should just return an empty event fragment and let the cda-caen-reader
 * application exit gracefully.
 */
extern bool g_stopAcquisition;

#endif // CDA_CORE_CAEN_STOPACQUISITION_H
