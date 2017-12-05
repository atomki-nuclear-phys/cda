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
    qttools5-dev-tools cmake make wget doxygen graphviz cernlib-base-dev \
    cernlib-core-dev libncurses5-dev \
    && apt-get clean

# Install ROOT:
RUN wget --no-check-certificate \
    https://root.cern.ch/download/root_v6.10.08.Linux-ubuntu16-x86_64-gcc5.4.tar.gz \
    && tar -C /usr/local --strip-components=1 --no-same-owner \
    -xvf root_v6.10.08.Linux-ubuntu16-x86_64-gcc5.4.tar.gz && \
    rm root_v6.10.08.Linux-ubuntu16-x86_64-gcc5.4.tar.gz

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
