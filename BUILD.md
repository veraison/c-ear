# Instructions

## Prerequisites

* CMake (>= 2.8)
* [libjwt](https://github.com/benmcollins/libjwt) 1.15.2 (which in turn depends on [jansson](https://github.com/akheron/jansson)):
* OpenSSL

### MacOSX

```bash
brew install cmake
brew install libjwt
brew install jansson
brew install openssl
```

### Ubuntu

> NOTE: not fully tested yet

```bash
sudo apt-get -y install cmake
```

* clone libjwt repo locally:

```bash
git clone --depth 1 --branch v1.15.2  https://github.com/benmcollins/libjwt.git
cd libjwt
```

* configure (jansson dependency is handled recursively):

```bash
cmake -Bbuild \
	-DUSE_INSTALLED_JANSSON=OFF \
	-DJANSSON_BUILD_DOCS=OFF
```

* build and install

```bash
cmake --build build --target all install
```

## Build, Test and Install

On MacOSX only:

```bash
export OPENSSL_ROOT_DIR=$(brew --prefix openssl)
```

The following does an out-of-source build in the `_build` folder, test and install using a custom prefix `_install`:

```bash
cmake -B_build -DCMAKE_INSTALL_PREFIX=_install
cmake --build _build/ --target all test install
```
