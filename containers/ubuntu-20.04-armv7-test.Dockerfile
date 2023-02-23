FROM arm32v7/ubuntu:20.04
RUN apt-get update && apt-get --yes install openssl ca-certificates
WORKDIR /usr/src