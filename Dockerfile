FROM tasp/dev:1.7.5-smolensk

COPY . /common
WORKDIR /common

RUN export DEBIAN_FRONTEND=noninteractive && \
    apt-get update && apt-get install -y --no-install-recommends --reinstall \
        libyaml-cpp-dev \
        libmsgsl-dev \
        libjsoncpp-dev

RUN mkdir build && cd build && cmake .. && ninja install
