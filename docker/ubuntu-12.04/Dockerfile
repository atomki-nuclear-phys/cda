#
# Ubuntu 12.04 image for building CDA on top of.
#

# Base the image on Ubuntu-12.04
FROM ubuntu:12.04

# Perform the installation as root:
USER root
WORKDIR /root

# Update the base image's package search path, to point at the legacy DEB
# repositories.
RUN sed -i -e "s/archive.ubuntu.com/old-releases.ubuntu.com/g" \
    /etc/apt/sources.list

# Install the required packages:
RUN apt-get update && apt-get -y install gcc g++ libqt4-dev make wget \
    cernlib-base-dev cernlib-core-dev libncurses5-dev && apt-get clean

# Install a newer version of CMake than what's available in this
# old release:
RUN wget --no-check-certificate \
    https://cmake.org/files/v3.12/cmake-3.12.4-Linux-x86_64.tar.gz \
    && tar -C /usr/local --strip-components=1 --no-same-owner \
       -xvf cmake-*.tar.gz \
    && rm cmake-*

# Install ROOT:
RUN wget --no-check-certificate \
    https://root.cern.ch/download/root_v5.34.36.Linux-ubuntu12-x86_64-gcc4.6.tar.gz \
    && tar -C /usr/local --strip-components=1 --no-same-owner \
       -xvf root_*.tar.gz && \
    rm root_*.tar.gz

# Install the CAEN libraries:
COPY CAENVMELib-2.50.tgz .
RUN tar -xvf CAENVMELib*.tgz && \
    cd CAENVMELib*/lib/ && sh install_x64 && \
    cd ../.. && rm -rf CAENVMELib*

COPY CAENComm-1.2-build20140211.tgz .
RUN tar -xvf CAENComm*.tgz && \
    cd CAENComm*/lib/ && sh install_x64 && \
    cd ../.. && rm -rf CAENComm*

COPY CAENDigitizer_2.9.1_20170707.tgz .
RUN tar -xvf CAENDigitizer*.tgz && \
    cd CAENDigitizer*/ && sh install_64 && \
    cd .. && rm -rf CAENDigitizer*

COPY CAENVMEToolBox.tar.xz .
RUN tar -xvf CAENVMEToolBox.tar.xz && \
    cd CAENVMEToolBox/CAENqtpSDK && sh install && \
    cd ../.. && rm -rf CAENVMEToolBox*

RUN chmod 644 /usr/include/CAEN* /usr/include/cvt_*

# Install the pcicc32 library:
COPY pcicc32-6.8.tar.gz .
RUN tar -xvf pcicc32*.tar.gz && \
    cd pcicc32*/ && make && make install && \
    cd .. && rm -rf pcicc32*

# Start with BASH by default:
CMD ["/bin/bash"]
