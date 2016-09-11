//
// Created by lee on 16-9-11.
//

#ifndef KQEVENT_IPADDRESS_H
#define KQEVENT_IPADDRESS_H
#include <string>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <memory>
#include <vector>

class IPAddress {
public:
    using IPAddressPtr = std::shared_ptr<IPAddress>;
    /*不推荐使用该方法获取IP地址，因为可能会造成阻塞*/
    static std::vector<IPAddressPtr> fromHostName(std::string const &host, in_port_t);
    static IPAddressPtr fromIPAddress(std::string const &addr, in_port_t port);
    static IPAddressPtr fromIPAddress(std::string const &addr);
    struct sockaddr_in getSockAddr();

    in_port_t getPort(){return _address.sin_port;}
    sa_family_t getFamily(){return _address.sin_family;}
    in_addr_t getAddr32(){return _address.sin_addr.s_addr;}

private:
    IPAddress(std::string const &address, in_port_t);
    IPAddress(struct sockaddr_in _address);

    struct sockaddr_in _address;
    socklen_t _len;
    std::string _addressStr;
};


#endif //KQEVENT_IPADDRESS_H
