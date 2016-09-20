#ifndef KQEVENT_ACCEPTOR_H
#define KQEVENT_ACCEPTOR_H

#include "AbstractAcceptor.h"
#include "IPAddress.h"

namespace KQEvent {
    class Acceptor : public AbstractAcceptor, public std::enable_shared_from_this<Acceptor> {
    public:
        using AcceptorPtr = std::shared_ptr<Acceptor>;

        Acceptor() = delete;

        template<typename ..._Args>
        static AcceptorPtr newInstance(_Args... &&args) {
            auto aNew = new Acceptor(std::forward<_Args>(args)...);
            return AcceptorPtr(aNew);
        }

        AcceptorPtr getPtr() { return shared_from_this(); }

        Connection::ConnectionPtr acceptHandle() override;

    private:
        Acceptor(Socket::SocketPtr socket, IPAddressPtr address);

        Socket::SocketPtr _socket;
        IPAddressPtr _address;
    };
}

#endif //KQEVENT_ACCEPTOR_H
