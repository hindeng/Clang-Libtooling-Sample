FROM ubuntu:16.04

WORKDIR /root

COPY Package /root/Package

RUN apt-get update && apt-get install -y cmake build-essential vim git python sudo

RUN cd /root && \
	git clone https://github.com/llvm/llvm-project.git && \
	cd /root/llvm-project && \
	git checkout release/6.x

RUN cd /root/llvm-project && \
	mkdir build && \
	cd build && \
	cmake -DLLVM_ENABLE_PROJECTS=clang -G "Unix Makefiles" ../llvm && \
	make 
