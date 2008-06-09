
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
 * The code is currently composed of one useful library that
 * should serve as a back-bone to the CDA application suit and some
 * example applications that use this library.
 *
 * The idea that the CAMAC devices will each be compiled in separate
 * Qt plugins. The beginnings of one such example plugin are already
 * available, but need finishing.
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
