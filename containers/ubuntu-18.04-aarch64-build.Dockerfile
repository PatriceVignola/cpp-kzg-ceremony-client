FROM ubuntu:18.04
ARG DEBIAN_FRONTEND=noninteractive
WORKDIR /usr/local
RUN dpkg --add-architecture arm64
RUN echo "deb [arch=amd64] http://archive.ubuntu.com/ubuntu bionic main universe restricted multiverse" >  /etc/apt/sources.list
RUN echo "deb [arch=amd64] http://archive.ubuntu.com/ubuntu bionic-updates main universe restricted multiverse" >>  /etc/apt/sources.list
RUN echo "deb [arch=amd64] http://archive.ubuntu.com/ubuntu bionic-security main universe restricted multiverse" >>  /etc/apt/sources.list
RUN echo "deb [arch=arm64] http://ports.ubuntu.com/ bionic main universe restricted multiverse" >> /etc/apt/sources.list
RUN echo "deb [arch=arm64] http://ports.ubuntu.com/ bionic-updates main universe restricted multiverse" >> /etc/apt/sources.list
RUN echo "deb [arch=arm64] http://ports.ubuntu.com/ bionic-security main universe restricted multiverse" >> /etc/apt/sources.list
RUN apt-get update && apt-get --yes install git wget make libc6-arm64-cross libc6-dev-arm64-cross gcc-aarch64-linux-gnu g++-aarch64-linux-gnu binutils-aarch64-linux-gnu libssl-dev:arm64
RUN wget https://github.com/Kitware/CMake/releases/download/v3.25.1/cmake-3.25.1-linux-x86_64.sh
RUN chmod +x cmake-3.25.1-linux-x86_64.sh
RUN ./cmake-3.25.1-linux-x86_64.sh --skip-license
RUN rm cmake-3.25.1-linux-x86_64.sh
RUN ln -s /usr/bin/aarch64-linux-gnu-ar /usr/local/bin/aarch64-linux-gnuar
WORKDIR /