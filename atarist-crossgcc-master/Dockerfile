FROM ubuntu:15.10

MAINTAINER konrad.mrozek@gmail.com

ADD atari-m68k.list /etc/apt/sources.list.d/atari-m68k.list
ADD atari-m68k-repo.key /root/atari-m68k-repo.key

RUN apt-key add /root/atari-m68k-repo.key
RUN apt-get update -y && apt-get install -y cross-mint-essential make
