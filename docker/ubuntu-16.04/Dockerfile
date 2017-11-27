#
# Ubuntu 16.04 image for building CDA on top of.
#

# Base the image on Ubuntu-16.04
FROM ubuntu:16.04

# Perform the installation as root:
USER root
WORKDIR /root

# Install the required packages:
RUN apt-get update && apt-get -y install gcc g++ qt5-default cmake make wget \
    doxygen graphviz cernlib-base-dev cernlib-core-dev ncurses-base \
    root-system-bin root-system-common \
    && apt-get clean

# Start with BASH by default:
CMD /bin/bash
