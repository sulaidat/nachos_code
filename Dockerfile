FROM ubuntu:18.04

RUN apt-get update
RUN apt-get install build-essential -y
RUN dpkg --add-architecture i386 
RUN apt-get install gcc-multilib g++-multilib -y
RUN apt-get install lib32ncurses5 lib32z1 -y
RUN apt-get install gcc-4.8 gcc-4.8-multilib g++-4.8 g++-4.8-multilib -y
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 40
RUN update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 40

# install peda
RUN apt install git -y
RUN apt install gdb -y
RUN git clone https://github.com/longld/peda.git ~/peda
RUN echo "source ~/peda/peda.py" >> ~/.gdbinit

RUN mkdir /home/nachos

WORKDIR /home/nachos

RUN apt-get install wget -y
RUN wget https://www.fit.hcmus.edu.vn/~ntquan/os/assignment/nachos_40.tar.gz
RUN wget https://www.fit.hcmus.edu.vn/~ntquan/os/assignment/mips-decstation.linux-xgcc.gz
RUN wget https://www.fit.hcmus.edu.vn/~ntquan/os/assignment/nachos-gcc.diff.gz
RUN tar zxvf nachos_40.tar.gz
RUN tar zxvf mips-decstation.linux-xgcc.gz
RUN tar zxvf nachos-gcc.diff.gz
RUN patch -p0 < nachos-gcc.diff

WORKDIR /home/nachos/NachOS-4.0/code/build.linux

RUN apt-get install ed -y
RUN make depend
RUN make

WORKDIR /home/nachos/NachOS-4.0/coff2noff

RUN make

WORKDIR /home/nachos/NachOS-4.0/code/test

RUN make

WORKDIR /home/nachos/NachOS-4.0/mycode

EXPOSE 4000

CMD ["echo", "tuyendeptrai"]
