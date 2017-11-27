/**
 * @mainpage CDA
 *
 * @section CDAintro Introduction
 *
 * CDA is short for <strong>C</strong>ommon <strong>D</strong>ata
 * <strong>A</strong>cquisition. It is an all-round data online
 * system to be used by the Experimental Nuclear Physics department
 * of the ATOMKI.
 *
 * @section CDAoverview Code overview
 *
 * The code is composed of a number of separate libraries/frameworks
 * that provide the back-bone of the CDA applications, and a set of
 * plugin libraries that provide the device specific code for the
 * applications.
 *
 * The idea that the CAMAC/CAEN/VME devices are compiled in separate
 * Qt plugins. This way the main part of the code can stay agnostic
 * about the details of the devices.
 *
 * For compilation and usage instructions see the README files of the
 * code repository.
 */
