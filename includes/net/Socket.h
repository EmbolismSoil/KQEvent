#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <string>
#include <memory>
#include <stdint.h>
#include <sys/socket.h>
#include "IPAddress.h"
#include <utility>
#include <unistd.h>
#include <iostream>
#include "TCPInfo.h"
#include <thread>

namespace KQEvent {
    class Socket : public std::enable_shared_from_this<Socket> {
    public:
        using SocketPtr = std::shared_ptr<Socket>;

        //不能复制
        Socket(Socket const &) = delete;

        Socket const &operator=(Socket const &) = delete;

        virtual ~Socket() {
            // if(!_isClose)
            std::cout << "close socket , fd =  " << _fd  << ", peer = "
                      << getPeerAddr()->toString() << std::endl;

            int ret = ::close(_fd);

            if (_tid == std::this_thread::get_id()){
                std::cout << "error " << std::endl;
            }

            if (ret == 0){
                std::cout << "close socket sccuessfully in thread : " << std::this_thread::get_id() << std::endl;
            }else{
                __setErrorString(errno);
                std::cout << "close socket failed : " << getError2String() << std::endl;
            }

        }

        template<typename  ..._Args>
        static SocketPtr newInstance(_Args &&...args) {
            auto aNew = new Socket(std::forward<_Args>(args)...);
            return SocketPtr(aNew);
        }

        SocketPtr getPtr() { return shared_from_this(); }

        int getFd() { return _fd; }

        IPAddress::IPAddressPtr getIPAddress();

        int setNoDelay(bool on);

        int setReuseAddr(bool on);

        int setReusePort(bool on);

        int setKeepAlive(bool on);

        int bind(IPAddress::IPAddressPtr);

        int listen(int backlog = 65535);

        int accept(IPAddress::IPAddressPtr &peerAddr);

        int accept4(IPAddress::IPAddressPtr &peerAddr);

        int connect(IPAddress::IPAddressPtr const &serverAddr);

        int getSocketError();

        IPAddress::IPAddressPtr getPeerAddr();
        // void forceClose(){
        //     _isClose = true;
        //     ::close(_fd);
        // }

        std::string getError2String() {
            return _messageError;
        }

    private:
        explicit Socket(int domain, int type);

        explicit Socket(int fd);

        explicit Socket();

        //bool _isClose;

        void __setErrorString(int err);

        std::string _messageError;
        int _fd;

        std::thread::id _tid;
        //IPAddress::IPAddressPtr _address;
    };
}
#endif