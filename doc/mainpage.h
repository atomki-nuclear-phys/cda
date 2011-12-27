
/**
 * @mainpage CDA
 *
 * @author Attila Krasznahorkay Jr.
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
 * @section CDAcompile Compilation
 *
 * You need a recent version of Qt (qt.nokia.com), versions above
 * 4.0.0 should work equally well. In the main directory you have to
 * execute:
 *
 * <code>
 *  # source setup.[c]sh<br/>
 *  # /path/to/Qt/bin/qmake cda.pro<br/>
 *  # make
 * </code>
 *
 * to compile all libraries and examples. The compiled libraries/frameworks are put
 * under <strong>./lib</strong> the applications under <strong>./bin</strong>,
 * and the device plugins under <strong>./dev</strong>.
 * To set up your environment for running the examples, from any
 * directory exacute:
 *
 * <code>
 *  # source /path/to/CDA/setup.[c]sh
 * </code>
 *
 * <i>(You should select the file matching your shell...)</i> Enjoy!
 *
 * $Revision$
 * $Date$
 */
