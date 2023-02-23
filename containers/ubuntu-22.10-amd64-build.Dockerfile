FROM ubuntu:22.10
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get --yes install build-essential libssl-dev git cmake