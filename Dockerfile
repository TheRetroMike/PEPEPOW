FROM ubuntu:20.04
RUN apt-get update -y
RUN apt-get install wget build-essential -y
WORKDIR /opt/
RUN wget https://github.com/MattF42/PePe-core/releases/download/v2.8.1.1/PEPEPOW-v2.8.1.1-40b8862-release-x86_64-linux-gnu.tgz
RUN tar zxvf PEPEPOW-v2.8.1.1-40b8862-release-x86_64-linux-gnu.tgz
RUN mv PEPEPOWd /usr/bin/
RUN mv PEPEPOW-cli /usr/bin/
RUN rm PEPEPOW*
RUN wget https://raw.githubusercontent.com/TheRetroMike/rmt-nomp/master/scripts/blocknotify.c
RUN gcc blocknotify.c -o /usr/bin/blocknotify
CMD /usr/bin/PEPEPOWd -printtoconsole
