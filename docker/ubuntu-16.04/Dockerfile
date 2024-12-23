#
# Ubuntu 16.04 image for building CDA on top of.
#

# Base the image on Ubuntu-16.04
FROM ubuntu:16.04

# Perform the installation as root:
USER root
WORKDIR /root

# Install the required packages:
RUN apt-get update && apt-get -y install gcc g++ qt5-default \
    qttools5-dev-tools make wget doxygen graphviz cernlib-base-dev \
    cernlib-core-dev libncurses5-dev \
    && apt-get clean

# Install a newer version of CMake than what's available in this
# old release:
    RUN wget --no-check-certificate \
    https://cmake.org/files/v3.31/cmake-3.31.3-linux-x86_64.tar.gz \
    && tar -C /usr/local --strip-components=1 --no-same-owner \
       -xvf cmake-*.tar.gz \
    && rm cmake-*

# Install ROOT:
RUN wget --no-check-certificate \
    https://root.cern.ch/download/root_v6.10.08.Linux-ubuntu16-x86_64-gcc5.4.tar.gz \
    && tar -C /usr/local --strip-components=1 --no-same-owner \
    -xvf root_*.tar.gz && \
    rm root_*.tar.gz

# Install the CAEN libraries:
COPY CAENVMELib-2.50.tgz .
RUN tar -xvf CAENVMELib*.tgz && \
    cd CAENVMELib*/lib && sh install_x64 && \
    cd ../.. && rm -rf CAENVMELib*

COPY CAENComm-1.2-build20140211.tgz .
RUN tar -xvf CAENComm*.tgz && \
    cd CAENComm*/lib && sh install_x64 && \
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
