![](doc/KQLogoLong.png)
#KQEvent
![](https://img.shields.io/badge/version-v1.0-blue.svg) ![](https://img.shields.io/badge/build-passing-brightgreen.svg) ![https://github.com/EmbolismSoil/KQEvent/archive/master.zip](https://img.shields.io/badge/dowload-32k-brightgreen.svg)

KQEvent是一个异步非阻塞网络库,使用`C++11`实现。意图展示一个基本的异步非阻塞网络库的实现，最终会在此基础上实现一个`Http Server`和一个`RPC`框架。
KQEvent的设计目标是：**方便使用**

###示例:  
```cpp
auto ip = IPAddress("localhost:80");
auto Server = TCPServer::newInstance(IPaddress);
Server->setReadHandler([](ConnectionPtr con){
                            auto buffer = MemPoll::alloc(512);
                            TCPServer::read(_fd, buffer);
                            //your code
                            return ALIVE;
                        })
Server.run();
```

###将会实现的特性
- 用户可以从内存池中分配内存，内存池跟踪内存的使用情况，跟踪内存生命周期，提供丰富的调试信息，必要时自动回收。降低用户内存管理的难度。
- 实现性能统计，可以在调试模式自动导出统计数据的图表形式。
- 基于无锁结构设计的高性能线程池
- 向用户提供高效的零拷贝的工具集

### 下载安装
``` shell
mkdir build && cd build && cmake .. && make
```

###正在进行的工作
- 细化connection的结构，跟踪连接的生命周期，处理连接异常
 - connection关闭连接的时候提供强制关闭接口，让用户决定是否立即关闭
 - connection可以在析构的时候关闭连接
- 精确定时器，时区转换
- 线程池
- Acceptor

## License
GPLv2
