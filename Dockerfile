#FROM ubuntu:20.04
#RUN apt-get update -y
#RUN apt-get install wget build-essential -y
#WORKDIR /opt/
#RUN wget https://github.com/MattF42/PePe-core/releases/download/v2.8.1.1/PEPEPOW-v2.8.1.1-40b8862-release-x86_64-linux-gnu.tgz
#RUN tar zxvf PEPEPOW-v2.8.1.1-40b8862-release-x86_64-linux-gnu.tgz
#RUN mv PEPEPOWd /usr/bin/
#RUN mv PEPEPOW-cli /usr/bin/
#RUN rm PEPEPOW*
#RUN wget https://raw.githubusercontent.com/TheRetroMike/rmt-nomp/master/scripts/blocknotify.c
#RUN gcc blocknotify.c -o /usr/bin/blocknotify
#CMD /usr/bin/PEPEPOWd -printtoconsole

FROM ubuntu:18.04
RUN apt-get update -y
RUN DEBIAN_FRONTEND=noninteractive apt-get install software-properties-common build-essential libtool autotools-dev automake pkg-config libssl-dev libevent-dev bsdmainutils libboost-all-dev libminiupnpc-dev libzmq3-dev -y
RUN add-apt-repository ppa:bitcoin/bitcoin
RUN apt-get update -y
RUN apt-get install libdb4.8-dev libdb4.8++-dev -y
WORKDIR /app
COPY . .
RUN ./autogen.sh
RUN ./configure --enable-glibc-back-compat --enable-reduce-exports LDFLAGS=-static-libstdc++  CFLAGS="-march=native" CXXFLAGS="-march=native"
RUN make
RUN mv /app/src/PEPEPOWd /app/PEPEPOWd
RUN mv /app/src/PEPEPOW-cli /app/PEPEPOW-cli
CMD /app/PEPEPOWd -printtoconsole
