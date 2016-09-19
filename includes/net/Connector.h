//
// Created by lee on 16-9-19.
//

#ifndef KQEVENT_CONNECTOR_H
#define KQEVENT_CONNECTOR_H

#include <memory>
#include "Socket.h"
#include "../base/Subject.h"
#include "Connection.h"

namespace KQEvent{
    class Connector {
    public:
        /*
         *  关于socket的生命周期：
         *      当socket打开，但是建立尚未成功时，由其生命周期由connector管理，
         *      当建立成功后，socket的管理权将转移到Connection。
         * */
        using ConnectorPtr = std::shared_ptr<Connector>;
        using ConnectionPtr = Connection::ConnectionPtr;
        using SocketPtr = Socket::SocketPtr ;
        using SubjectPtr = Subject::SubjectPtr;
        using ObserverPtr = Observer::ObserverPtr;
        using SuccessHandle_t = std::function<void(ConnectionPtr)>;
        using ErrorHandle_t = std::function<void(SocketPtr, int)>;


        Connector(Connector const&) = delete;
        Connector const &operator=(Connector const&) = delete;

        template <typename ..._Args>
        static  ConnectorPtr newInstance(_Args&& ...args){
            auto aNew = new Connector(std::forward<_Args>(args)...);
            return ConnectorPtr(aNew);
        }

        SubjectPtr getSubject(){return _writeSubject;}
        void setSucessHandler(SuccessHandle_t);
        void setErrorHandler(ErrorHandle_t);

        void setSocket(SocketPtr sock){
            _socket = sock;
        }

        void setServerAddr(IPAddress::IPAddressPtr addr){
            _serverAddr = addr;
        }

        void connect();
    private:
        explicit Connector(std::string const& serverAddr);

        Observer::Command_t _handleWrite(SubjectPtr sub);

        SocketPtr _socket;
        SubjectPtr _writeSubject;
        ObserverPtr _writeObserver;
        SuccessHandle_t _onConnectedHanle;
        ErrorHandle_t _onError;
        IPAddress::IPAddressPtr _serverAddr;
    };
}



#endif //KQEVENT_CONNECTOR_H
