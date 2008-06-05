
/**
 * @mainpage CDA
 *
 * @author Attila Krasznahorkay Jr.
 *
 * @section CDAintro Introduction
 *
 * CDA is short for <strong>C</strong>AMAC <strong>D</strong>ata
 * <strong>A</strong>cquisition. It is an all-round data online
 * system to be used by the Experimental Nuclear Physics department
 * of the ATOMKI.
 *
 * @section CDAoverview Code overview
 *
 * The code is currently composed of a few useful libraries that
 * should serve as a back-bone to the CDA application suit and some
 * example applications that use these libraries. The current
 * libraries are:
 *
 *   - <strong>cdamsg</strong>: A library that handles messages coming
 *     from various
 *     parts of the data acquisition system. The messages are
 *     transmitted from the components to a(multiple) server(s)
 *     over TCP/IP protocol to enable running the different parts of
 *     CDA on separate machines.
 *   - <strong>cdafifo</strong>: A very "thin" library built around
 *     the QFile class. It just demonstrates how to use U*IX file-system
 *     FIFOs for transmitting data between processes.
 *
 * @section CDAcompile Compilation
 *
 * You need a recent version of Qt (www.trolltech.com), versions above
 * 4.0.0 should work equally well. In the main directory you have to
 * execute:
 *
 * <code>
 *  # /path/to/Qt/bin/qmake cda.pro<br/>
 *  # make
 * </code>
 *
 * to compile all libraries and examples. The compiled libraries are put
 * under <strong>./lib</strong> and the examples under <strong>./bin</strong>.
 * To set up your environment for running the examples, in the main CDA
 * directory exacute:
 *
 * <code>
 *  # source setup.[c]sh
 * </code>
 *
 * <i>(You should select the file matching your shell...)</i> Enjoy!
 *
 * $Revision$
 * $Date$
 */
