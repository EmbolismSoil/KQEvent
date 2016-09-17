#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <string>
#include <memory>
#include <stdint.h>
#include <sys/socket.h>
#include "IPAddress.h"
#include <utility>
#include <unistd.h>

namespace KQEvent {
    class Socket : public std::enable_shared_from_this<Socket> {
    public:
        using SocketPtr = std::shared_ptr<Socket>;

        Socket() = delete;

        //不能复制
        Socket(Socket const &) = delete;

        Socket const &operator=(Socket const &) = delete;

        virtual ~Socket(){
            ::close(_fd);
        }

         template <typename  ..._Args>
         static SocketPtr newInstance(_Args&& ...args){
             auto aNew = new Socket(std::forward<_Args>(args)...);
             return SocketPtr(aNew);
         }

        SocketPtr getPtr() { return shared_from_this(); }

        int getFd(){return _fd;}

        IPAddress::IPAddressPtr getIPAddress();

        int setNoDelay(bool on);

        int setReuseAddr(bool on);

        int setReusePort(bool on);

        int setKeepAlive(bool on);

        int bind(IPAddress::IPAddressPtr);

        int listen(int backlog);

        int accept(IPAddress::IPAddressPtr &peerAddr);

        std::string getError2String() {
            return _messageError;
        }

    private:
        Socket(int domain, int type);
        Socket(int fd);

        void __setErrorString(int err);

        std::string _messageError;
        int _fd;
        //IPAddress::IPAddressPtr _address;
    };
}
#endif