FROM gcc:8.3.0 as readmrz_base

WORKDIR /projects

RUN apt-get update && \
    apt-get install -y --no-install-recommends --fix-missing \
        ninja-build \
        wget  \
        unzip \
        zip \
        git && \
    rm -rf \
        /var/cache/apt/archives/* \
        /var/lib/apt/lists/* \
        /tmp/* \
        /var/tmp/* \
        /var/lib/apt/lists/* \
        /usr/local/src/* && \
    git clone https://github.com/microsoft/vcpkg.git && \
    git clone https://github.com/AntonioCS/readMRZ.git && \
    cd vcpkg/ && \
    ./bootstrap-vcpkg.sh
    #&& \
#    ./vcpkg install opencv && \
#    ./vcpkg install tesseract

ARG CMAKE_VERSION=3.19.2

RUN wget https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-Linux-x86_64.sh \
      -q -O /tmp/cmake-install.sh \
      && chmod u+x /tmp/cmake-install.sh \
      && mkdir /usr/bin/cmake \
      && /tmp/cmake-install.sh --skip-license --prefix=/usr/bin/cmake \
      && rm /tmp/cmake-install.sh

ENV PATH="/usr/bin/cmake/bin:${PATH}"
ENV CXXFLAGS=-g
ENV VCPKG_BINARY_SOURCES=clear
ENV CFLAGS=-g

RUN cd /projects/readMRZ && \
    mkdir build && \
    cd build && \
    cmake -DVCPKG_TARGET_TRIPLET=x64-linux -DREADMRZ_ENABLE_TESTS=off -DCMAKE_BUILD_TYPE=Release  -DCMAKE_TOOLCHAIN_FILE=/projects/vcpkg/scripts/buildsystems/vcpkg.cmake .. && \
    cmake --build . --target readMRZcli

#FROM alpine:latest as readmrz_prod
#COPY --from=readmrz_base /projects/readMRZ/build/cli/readMRZcli /projects/
