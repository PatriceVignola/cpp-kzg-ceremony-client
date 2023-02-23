FROM ubuntu:20.04
ARG DEBIAN_FRONTEND=noninteractive
RUN dpkg --add-architecture armhf
RUN echo "deb [arch=amd64] http://archive.ubuntu.com/ubuntu focal main universe restricted multiverse" >  /etc/apt/sources.list
RUN echo "deb [arch=amd64] http://archive.ubuntu.com/ubuntu focal-updates main universe restricted multiverse" >>  /etc/apt/sources.list
RUN echo "deb [arch=amd64] http://archive.ubuntu.com/ubuntu focal-security main universe restricted multiverse" >>  /etc/apt/sources.list
RUN echo "deb [arch=armhf] http://ports.ubuntu.com/ focal main universe restricted multiverse" >> /etc/apt/sources.list
RUN echo "deb [arch=armhf] http://ports.ubuntu.com/ focal-updates main universe restricted multiverse" >> /etc/apt/sources.list
RUN echo "deb [arch=armhf] http://ports.ubuntu.com/ focal-security main universe restricted multiverse" >> /etc/apt/sources.list
RUN apt-get update && apt-get --yes install git cmake libc6-armel-cross libc6-dev-armel-cross gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf binutils-arm-linux-gnueabi libssl-dev:armhf