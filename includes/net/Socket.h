#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <string>
#include <memory>

class Socket : public std::enable_shared_from_this<Socket>{
public:
    Socket(std::string const &name):_name(name){}
    std::shared_ptr<Socket> getPtr(){return shared_from_this();}

    Socket(Socket const &) = delete;
    Socket const &operator=(Socket const &) = delete;

private:
    std::string _name;
};

#endif