// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_FIFO_FIFO_H
#define CDA_CORE_FIFO_FIFO_H

// Qt include(s):
#include <QtCore/QFile>
#include <QtCore/QCoreApplication>

/**
 *  @short Light-weight layer over QFile for handling FIFOs
 *
 *         The U*IX file-system FIFOs can be handled just as normal
 *         files. This thin layer only makes sure that they are
 *         always opened in read-write mode, as that's the only way
 *         that they can work. The class does provide the full
 *         functionality of a QFile, but really only the QIODevice
 *         interface of it should be used.
 *
 * @author Attila Krasznahorkay Jr.
 *
 * $Revision$
 * $Date$
 */
class Fifo : public QFile {

   Q_OBJECT

public:
   /// Constructor with FIFO path and Qt parent
   Fifo( const QString& path = "dummy", QObject* parent = 0 );
   /// Destructor
   virtual ~Fifo();

   /// Simplified open function not expecting any parameters
   virtual bool open( QIODevice::OpenMode );

}; // class Fifo

#endif // CDA_CORE_FIFO_FIFO_H
