# About this Repository
This is a dockerfile for clanglibtooling sample including a tool to modify source code. Our goal is to use clang libtooling to modify the types of the target variables in `/Package/test.c` . 

Goal
-------------------------------------- 
This is `test.c`:

```c++
//The header is commented because handling header needs additional effort.
//#include <stdio.h>

void do_math(int *x) {
  double target3 = 1;   // need to change to int
  *x += 5;
}

int main(void) {
  double result = -1;
  long double target = 2.0;   //need to change to double
  int target2 = 2;   //need to change to long double
  int val = 4;
  do_math(&val);
  printf("abc\n");
  return result;
}
```

This is what we want our final code would be.

```c++
//The header is commented because handling header needs additional effort.
//#include <stdio.h>

void do_math(int *x) {
  int target3 = 1;   // need to change to int
  *x += 5;
}

int main(void) {
  double result = -1;
  double target = 2.0;   //need to change to double
  long double target2 = 2;   //need to change to long double
  int val = 4;
  int val = 4;
  do_math(&val);
  printf("abc\n");
  return result;
}
```

Prerequisites  
---------------------------------------  
To use this sample, Docker are required to be installed first，  
1. Install Docker: https://www.docker.com/products/docker-desktop

Usage  
--------------------------------------  
1. Get the docker image. There are two ways to do this .<br/> 

   ① Build the image locally
      1.  Clone this repository. <br/>
      2.  Change to `Clang-Libtooling-Sample` directory. <br/>
      3.  Run your Docker application. I would recommend to maximize the swap and memory that docker is able to use.        Otherwise, docker may crash while building image. You can find this setting in Preferences->Advanced. I used 12GiB memory and 4GiB swap. You may also want to increase the disk image size to about 100GiB(It is in Preferences->Disk) <br/>
      4.  Run `docker build -t 474460783/libtoolingsample .` . <br/>
   
   ② Pull my image from docker hub
      1. Or you could run `docker pull 474460783/libtoolingsample`. <br/>

2. Run `docker run -it 474460783/libtoolingsample`.

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


