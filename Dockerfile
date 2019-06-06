FROM ubuntu:18.04

# Install dev environment
RUN apt-get update
RUN apt-get upgrade -y

RUN apt-get install -y curl wget git libcurl4-gnutls-dev
RUN apt-get install -y build-essential 
RUN apt-get install -y cmake libc6-dev

WORKDIR /usr/src/libgeoda

# Copy the current directory contents into the container at WORKDIR
COPY . .

RUN \
    cd src && \
    rm geoda && \
    git clone https://github.com/lixun910/geoda.git && \
    cd geoda && \
    git checkout libgeoda && \
    cd .. && \
    cd ..

RUN \
    rm -rf build && \
    mkdir build && \
    cd build
    #cmake -DCMAKE_INSTALL_PREFIX:PATH=./deps/osx/libgeoda .. && \
    #make && \
    #make install



