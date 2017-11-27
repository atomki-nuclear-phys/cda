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
    cernlib-base-dev cernlib-core-dev && apt-get clean

# Install a (very slightly) newer version of CMake than what's available in this
# old release:
RUN wget --no-check-certificate \
    https://cmake.org/files/v2.8/cmake-2.8.11.tar.gz \
    && tar -xvf cmake-2.8.11.tar.gz \
    && cd cmake-2.8.11/ && ./bootstrap && make && make install \
    && cd ../ && rm -rf cmake-2.8.11*

# Start with BASH by default:
CMD /bin/bash
