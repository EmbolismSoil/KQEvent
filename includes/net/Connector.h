//
// Created by lee on 16-9-19.
//

#ifndef KQEVENT_CONNECTOR_H
#define KQEVENT_CONNECTOR_H

#include <memory>
#include "Socket.h"
#include "../base/Subject.h"
#include "Connection.h"
#include "../base/TimerTaskQueue.h"
#include "../base/EventLoop.h"

namespace KQEvent{
    class Connector {
    public:
        /*
         *  关于socket的生命周期：
         *      当socket打开，但是建立尚未成功时，由其生命周期由connector管理，
         *      当建立成功后，socket的管理权将转移到Connection。
         * */
        typedef enum{RETRY = 0, STOP} optionE;
        using ConnectorPtr = std::shared_ptr<Connector>;
        using ConnectionPtr = Connection::ConnectionPtr;
        using SocketPtr = Socket::SocketPtr ;
        using SubjectPtr = Subject::SubjectPtr;
        using ObserverPtr = Observer::ObserverPtr;
        using SuccessHandle_t = std::function<void(ConnectionPtr)>;
        using ErrorHandle_t = std::function<optionE(SocketPtr, int)>;
        using TimerTaskQueuePtr =  TimerTaskQueue::TimerTaskQueuePtr;
        using EventLoopPtr = EventLoop::EventLoopPtr;
        using IPAddressPtr = IPAddress::IPAddressPtr;


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

        void setMaxRetry(int n){
            _maxRetry = n;
        }

        void connect();
    private:
        explicit Connector(EventLoopPtr loop,
                           std::string const& serverAddr,
                           std::string const& localAddr = std::string(),
                           int maxRetry = 5);

        void __retry();
        Observer::Command_t _handleWrite(SubjectPtr sub);

        SocketPtr _socket;
        SubjectPtr _writeSubject;
        ObserverPtr _writeObserver;
        SuccessHandle_t _onConnectedHanle;
        ErrorHandle_t _onError;
        IPAddress::IPAddressPtr _serverAddr;
        IPAddress::IPAddressPtr _localAddr;
        EventLoopPtr _loop;
        bool _isConnected;
        int _retryCount;
        int _maxRetry;
    };
}



#endif //KQEVENT_CONNECTOR_H
