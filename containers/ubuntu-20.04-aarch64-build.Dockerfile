FROM ubuntu:20.04
ARG DEBIAN_FRONTEND=noninteractive
RUN dpkg --add-architecture arm64
RUN echo "deb [arch=amd64] http://archive.ubuntu.com/ubuntu focal main universe restricted multiverse" >  /etc/apt/sources.list
RUN echo "deb [arch=amd64] http://archive.ubuntu.com/ubuntu focal-updates main universe restricted multiverse" >>  /etc/apt/sources.list
RUN echo "deb [arch=amd64] http://archive.ubuntu.com/ubuntu focal-security main universe restricted multiverse" >>  /etc/apt/sources.list
RUN echo "deb [arch=arm64] http://ports.ubuntu.com/ focal main universe restricted multiverse" >> /etc/apt/sources.list
RUN echo "deb [arch=arm64] http://ports.ubuntu.com/ focal-updates main universe restricted multiverse" >> /etc/apt/sources.list
RUN echo "deb [arch=arm64] http://ports.ubuntu.com/ focal-security main universe restricted multiverse" >> /etc/apt/sources.list
RUN apt-get update && apt-get --yes install git cmake libc6-arm64-cross libc6-dev-arm64-cross gcc-aarch64-linux-gnu g++-aarch64-linux-gnu binutils-aarch64-linux-gnu libssl-dev:arm64