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

namespace KQEvent {

    class IPAddress : public std::enable_shared_from_this<IPAddress> {
    public:
        using IPAddressPtr = std::shared_ptr<IPAddress>;

        /*不推荐使用该方法获取IP地址，因为可能会造成阻塞*/
        static std::vector<IPAddressPtr> fromHostName(std::string const &host, in_port_t = 80);

        static IPAddressPtr fromIPAddress(std::string const &addr, in_port_t port);

        static IPAddressPtr fromIPAddress(std::string const &addr);

        static IPAddressPtr addrAny(in_port_t port = 0);

        IPAddressPtr getPtr() { return shared_from_this(); }

        struct sockaddr_in getSockAddr() { return _address; }

        in_port_t getPort() { return _address.sin_port; }

        sa_family_t getFamily() { return _address.sin_family; }

        in_addr_t getAddr32() { return _address.sin_addr.s_addr; }

        std::string const &toString() { return _addressStr; }

    private:
        IPAddress(std::string const &address, in_port_t);

        IPAddress(struct sockaddr_in address);

        struct sockaddr_in _address;
        socklen_t _len;
        std::string _addressStr;
    };
}

#endif //KQEVENT_IPADDRESS_H
