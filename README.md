# Http_Framework

使用Boost Asio和c++20 coroutine搭建，基于C++的简单http服务。

### how to build (example on MacOS)

~~~
mkdir build && cd build
~~~

~~~
conan install .. --build=missing -s build_type=Release -s compiler.cppstd=20
~~~

~~~
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=build/Release/generators/conan_toolchain.cmake
~~~

~~~
make
~~~