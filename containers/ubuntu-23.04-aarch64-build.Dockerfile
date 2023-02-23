FROM ubuntu:23.04
ARG DEBIAN_FRONTEND=noninteractive
RUN dpkg --add-architecture arm64
RUN cat /etc/apt/sources.list
RUN echo "deb [arch=amd64] http://archive.ubuntu.com/ubuntu lunar main universe restricted multiverse" >  /etc/apt/sources.list
RUN echo "deb [arch=amd64] http://archive.ubuntu.com/ubuntu lunar-updates main universe restricted multiverse" >>  /etc/apt/sources.list
RUN echo "deb [arch=amd64] http://archive.ubuntu.com/ubuntu lunar-security main universe restricted multiverse" >>  /etc/apt/sources.list
RUN echo "deb [arch=arm64] http://ports.ubuntu.com/ lunar main universe restricted multiverse" >> /etc/apt/sources.list
RUN echo "deb [arch=arm64] http://ports.ubuntu.com/ lunar-updates main universe restricted multiverse" >> /etc/apt/sources.list
RUN echo "deb [arch=arm64] http://ports.ubuntu.com/ lunar-security main universe restricted multiverse" >> /etc/apt/sources.list
RUN apt-get update && apt-get --yes install git cmake libc6-arm64-cross libc6-dev-arm64-cross gcc-aarch64-linux-gnu g++-aarch64-linux-gnu binutils-aarch64-linux-gnu libssl-dev:arm64