# Clang Libtooling Sample
A docker image for clanglibtooling including a tool to modify source code

# About this Repository
A Dockerfile for LibtoolingSameple.


Prerequisites  
---------------------------------------  
To use this sample, Docker are required to be installed first，  
1. Install Docker: https://www.docker.com/products/docker-desktop

Usage  
--------------------------------------  
1. Clone this repository.
2. Change to `xuan-project` directory.
3. Run your Docker application. I would recommend to maximize the swap and memory that docker is able to use. Otherwise, docker may crash while building image. You can find this setting in Preferences->Advanced. I used 12GiB memory and 4GiB swap. You may also want to increase the disk image size to about 100GiB(It is in Preferences->Disk)
4. Run `docker build -t libtoolingsample .`.(This step takes several hours to complete. It took me 4.5 hours.)
5. Run `docker run -it libtoolingsample`.

# Running Demo

```
cd /root/Package
make clean
make
chmod 777 run.sh
./run.sh
```

You can see the oiriginal code and json config by using these commands
```
cat test.c
cat config.json
```


