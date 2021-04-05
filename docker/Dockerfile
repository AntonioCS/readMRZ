FROM rikorose/gcc-cmake:latest as readmrz_base

WORKDIR /projects

RUN apt-get update && \
    apt-get install -y --no-install-recommends --fix-missing \
        ninja-build \
        wget  \
        unzip \
        zip && \
    rm -rf \
        /var/cache/apt/archives/* \
        /var/lib/apt/lists/* \
        /tmp/* \
        /var/tmp/* \
        /var/lib/apt/lists/* \
        /var/www/html \
        /usr/local/src/* && \
    git clone https://github.com/microsoft/vcpkg.git && \
    git clone https://github.com/AntonioCS/readMRZ.git && \
    cd vcpkg/ && \
    ./bootstrap-vcpkg.sh

RUN cd /projects/readMRZ && \
    mkdir build && \
    cd build && \
    cmake -DVCPKG_TARGET_TRIPLET=x64-linux -DREADMRZ_ENABLE_TESTS=off -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=/projects/vcpkg/scripts/buildsystems/vcpkg.cmake .. && \
    cmake --build . --target=readMRZcli
