FROM ubuntu:18.04
ARG DEBIAN_FRONTEND=noninteractive
WORKDIR /usr/local
RUN dpkg --add-architecture armhf
RUN echo "deb [arch=amd64] http://archive.ubuntu.com/ubuntu bionic main universe restricted multiverse" >  /etc/apt/sources.list
RUN echo "deb [arch=amd64] http://archive.ubuntu.com/ubuntu bionic-updates main universe restricted multiverse" >>  /etc/apt/sources.list
RUN echo "deb [arch=amd64] http://archive.ubuntu.com/ubuntu bionic-security main universe restricted multiverse" >>  /etc/apt/sources.list
RUN echo "deb [arch=armhf] http://ports.ubuntu.com/ bionic main universe restricted multiverse" >> /etc/apt/sources.list
RUN echo "deb [arch=armhf] http://ports.ubuntu.com/ bionic-updates main universe restricted multiverse" >> /etc/apt/sources.list
RUN echo "deb [arch=armhf] http://ports.ubuntu.com/ bionic-security main universe restricted multiverse" >> /etc/apt/sources.list
RUN apt-get update && apt-get --yes install git wget make libc6-armel-cross libc6-dev-armel-cross gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf binutils-arm-linux-gnueabi libssl-dev:armhf
RUN wget https://github.com/Kitware/CMake/releases/download/v3.25.1/cmake-3.25.1-linux-x86_64.sh
RUN chmod +x cmake-3.25.1-linux-x86_64.sh
RUN ./cmake-3.25.1-linux-x86_64.sh --skip-license
RUN rm cmake-3.25.1-linux-x86_64.sh
RUN ln -s /usr/bin/arm-linux-gnueabihf-ar  /usr/local/bin/arm-linux-gnueabihfar
WORKDIR /