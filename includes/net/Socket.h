#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <string>
#include <memory>
#include <stdint.h>
#include <sys/socket.h>

class Socket : public std::enable_shared_from_this<Socket>{
public:
    Socket(std::string const &name, std::string const &addr);
    std::shared_ptr<Socket> getPtr(){return shared_from_this();}

    Socket(Socket const &) = delete;
    Socket const &operator=(Socket const &) = delete;

private:
    std::string const _name;
    struct sockaddr _sock;
    socklen_t _sockLen;
};

#endif