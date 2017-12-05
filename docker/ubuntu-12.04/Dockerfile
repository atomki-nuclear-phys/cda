#
# Ubuntu 12.04 image for building CDA on top of.
#

# Base the image on Ubuntu-12.04
FROM ubuntu:12.04

# Perform the installation as root:
USER root
WORKDIR /root

# Install the required packages:
RUN apt-get update && apt-get -y install gcc g++ libqt4-dev make wget \
    cernlib-base-dev cernlib-core-dev libncurses5-dev && apt-get clean

# Install a (very slightly) newer version of CMake than what's available in this
# old release:
RUN wget --no-check-certificate \
    https://cmake.org/files/v2.8/cmake-2.8.11.tar.gz \
    && tar -xvf cmake-2.8.11.tar.gz \
    && cd cmake-2.8.11/ && ./bootstrap && make && make install \
    && cd ../ && rm -rf cmake-2.8.11*

# Install ROOT:
RUN wget --no-check-certificate \
    https://root.cern.ch/download/root_v5.34.36.Linux-ubuntu12-x86_64-gcc4.6.tar.gz \
    && tar -C /usr/local --strip-components=1 --no-same-owner \
    -xvf root_v5.34.36.Linux-ubuntu12-x86_64-gcc4.6.tar.gz && \
    rm root_v5.34.36.Linux-ubuntu12-x86_64-gcc4.6.tar.gz

# Install the CAEN libraries:
COPY CAENVMELib-2.50.tgz .
RUN tar -xvf CAENVMELib-2.50.tgz && \
    cd CAENVMELib-2.50/lib && sh install_x64 && \
    cd ../.. && rm -rf CAENVMELib-2.50*

COPY CAENComm-1.2-build20140211.tgz .
RUN tar -xvf CAENComm-1.2-build20140211.tgz && \
    cd CAENComm-1.2/lib && sh install_x64 && \
    cd ../.. && rm -rf CAENComm-1.2*

COPY CAENDigitizer_2.9.1_20170707.tgz .
RUN tar -xvf CAENDigitizer_2.9.1_20170707.tgz && \
    cd CAENDigitizer_2.9.1 && sh install_64 && \
    cd .. && rm -rf CAENDigitizer_2.9.1*

COPY CAENVMEToolBox.tar.xz .
RUN tar -xvf CAENVMEToolBox.tar.xz && \
    cd CAENVMEToolBox/CAENqtpSDK && sh install && \
    cd ../.. && rm -rf CAENVMEToolBox*

RUN chmod 644 /usr/include/CAEN* /usr/include/cvt_*

# Start with BASH by default:
CMD /bin/bash
