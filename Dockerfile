FROM ubuntu:18.04

# Install dev environment
RUN apt-get update
RUN apt-get upgrade -y

RUN apt-get install -y curl wget git libcurl4-gnutls-dev
RUN apt-get install -y build-essential 
RUN apt-get install -y cmake libc6-dev

# install libiconv
RUN \
    cd /tmp && \
    wget http://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.13.tar.gz && \
    tar -xf libiconv-1.13.tar.gz && \
    cd libiconv-1.13/ && \
    ./configure --enable-static && \
    make && \
    make install

# install libcurl
RUN \
    cd /tmp && \
    wget https://curl.haxx.se/download/curl-7.61.0.tar.gz && \
    tar -xf curl-7.61.0.tar.gz && \
    cd curl-7.61.0/ && \
    ./configure && \
    make && \
    make install

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



