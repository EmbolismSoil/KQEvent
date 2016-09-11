//
// Created by lee on 16-9-11.
//

#include "IPAddress.h"
#include <arpa/inet.h>
#include <netdb.h>

IPAddress::IPAddress(std::string const &address, in_port_t port) :
        _len(sizeof(struct sockaddr_in)) {
    in_addr_t addr = inet_addr(address.c_str());

    this->_address.sin_addr.s_addr = addr;
    this->_address.sin_port = htons(port);
    this->_address.sin_family = AF_INET;
}

IPAddress::IPAddressPtr IPAddress::fromIPAddress(std::string const &addr, in_port_t port) {
    auto aNew = new IPAddress(addr, port);
    return IPAddress::IPAddressPtr(aNew);
}

IPAddress::IPAddressPtr IPAddress::fromIPAddress(std::string const &addr) {
    auto pos = addr.find('c');
    auto address = addr.substr(0, pos);
    auto port = addr.substr(pos + 1);
    in_port_t port16 = static_cast<in_port_t >(atoi(port.c_str()));
    auto aNew = new IPAddress(address, port16);
    return IPAddress::IPAddressPtr(aNew);
}

std::vector<IPAddress::IPAddressPtr> IPAddress::fromHostName(std::string const &hostName, in_port_t port) {
    auto hosts = gethostbyname(hostName.c_str());
    //目前只支持IPV4
    if (hosts == NULL && hosts->h_addrtype != AF_INET) {
        return std::vector();
    }

    std::vector<IPAddressPtr> ret;
    struct sockaddr_in sock_addr;
    for (auto pptr = hosts->h_addr_list; *pptr != NULL; ++pptr) {
        sock_addr.sin_addr = *((in_addr *) (*pptr));
        sock_addr.sin_port = port;
        sock_addr.sin_family = hosts->h_addrtype;
        auto aNew = new IPAddress(sock_addr);
        ret.push_back(IPAddressPtr(aNew));
    }

    return ret;
}

IPAddress::IPAddress(struct sockaddr_in address) :
        _len(sizeof(struct sockaddr_in)),
        _address(address)
{

}
