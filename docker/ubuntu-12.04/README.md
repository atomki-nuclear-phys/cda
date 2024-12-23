Ubuntu 12.04 Build Image Configuration
======================================

This configuration is used to create a build image that tests that the code
can still be successfully built on top of Ubuntu 12.04. (The OS used on the
data acquisition machine at the moment.)

Build the image (for deployment) with:

```
docker build --compress -t registry.gitlab.com/atomki-nuclear-phys/cda/reference-os:ubuntu-12.04-vX .
```
