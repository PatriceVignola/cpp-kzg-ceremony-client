FROM ubuntu:18.04
ARG DEBIAN_FRONTEND=noninteractive
WORKDIR /usr/local
RUN apt-get update && apt-get --yes install build-essential libssl-dev git wget
RUN wget https://github.com/Kitware/CMake/releases/download/v3.25.1/cmake-3.25.1-linux-x86_64.sh
RUN chmod +x cmake-3.25.1-linux-x86_64.sh
RUN ./cmake-3.25.1-linux-x86_64.sh --skip-license
RUN rm cmake-3.25.1-linux-x86_64.sh
WORKDIR /