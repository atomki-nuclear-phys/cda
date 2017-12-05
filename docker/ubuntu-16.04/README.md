Ubuntu 16.04 Build Image Configuration
======================================

This configuration is used to create a build image that tests that the code
can be successfully built on top of Ubuntu 16.04. (The latest LTS release of
Ubuntu at the time of writing.)

Build the image (for deployment) with:

```
docker build --compress --squash -t krasznaa/cda-build:ubuntu-16.04 .
```