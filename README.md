ATOMKI Common Data Acquisition
==============================

This project provides data acquisition for the experimental nuclear physics
setups used by the
[Laboratory of Nuclear Physics](http://www.atomki.hu/en/departments/2/introduction)
of the [Institute For Nuclear Research](http://www.atomki.hu/en/) of the
[Hungarian Academy of Sciences](http://mta.hu/english).

It is able to collect correlated "event data" from a collection of devices owned
by the laboratory, stream the collected data between separate processes,
possibly running on separate physical machines, monitor the data during the data
taking, and write the collected data in various formats
([HBOOK](http://cern.ch/cernlib) and [ROOT](http://root.cern.ch)) to disk.

Building the Code
-----------------

The project's build is configured using [CMake](https://cmake.org), in a fairly
standard way. In principle both in-source and out-of-source builds should be
possible, but out-of-source building is highly recommended. Like:

```
mkdir cda-build
cd cda-build/
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_PREFIX=~/CDA/ ../cda/
make
[sudo] make install
```
