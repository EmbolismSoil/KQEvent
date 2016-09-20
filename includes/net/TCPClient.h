//
// Created by lee on 16-9-19.
//

#ifndef KQEVENT_TCPCLIENT_H
#define KQEVENT_TCPCLIENT_H

#include <memory>
#include "Connection.h"
#include "Socket.h"
#include "Connector.h"

namespace KQEvent {
    class TCPClient {
    public:
        using TCPClientPtr = std::shared_ptr<TCPClient>;
        using ConnectionPtr = Connection::ConnectionPtr;
        using ConnectorPtr = Connector::ConnectorPtr;
        using IPAddressPtr = IPAddress::IPAddressPtr;
        using EventLoopPtr = EventLoop::EventLoopPtr;
        using SocketPtr = Socket::SocketPtr;
        using Handle_t = Connector::SuccessHandle_t;
        using ReadHandle_t = std::function<void(char *, size_t)>;
        using CloseHandle_t = Connection::CloseHandle_t;
        using ErrorHandle_t = std::function<Connector::optionE(SocketPtr, int)>;
        using ExceptHandle_t = Connection::ExceptHandle_t;

        TCPClient(TCPClient const &) = delete;

        TCPClient const &operator=(TCPClient const &) = delete;

        template<typename  ..._Args>
        static TCPClientPtr newInstance(_Args ...args) {
            auto aNew = new TCPClient(std::forward<_Args>(args)...);
            return TCPClientPtr(aNew);
        }

        void onConnected(Handle_t handle) {
            _onConnectedCallback = handle;
        }

        void onRead(ReadHandle_t handle) {
            _onReadCallback = handle;
        }

        void onClose(CloseHandle_t handle) {
            _onCloseCallback = handle;
        }

        //已建立连接出现异常
        void onConnectionExcept(ExceptHandle_t handle) {
            _onExceptCallback = handle;
        }

        //连接时出错
        void onConnectError(ErrorHandle_t handle) {
            _onConnectErrorCallBack = handle;
        }

        void setMaxRetry(int n) {
            _maxRetry = n;
        }

        bool sendMsg(char *msg, size_t len);

        void run(void) {
            _loop->loop();
        }

    private:
        explicit TCPClient(std::string const &serverAddr,
                           std::string const &localAddr = std::string());

        void __onConnected(ConnectionPtr conn);

        void __onClose(ConnectionPtr conn);

        Connector::optionE __onError(SocketPtr socket, int err);

        void __onExcept(ConnectionPtr conn);

        void __onRead(ConnectionPtr, char *, size_t);

        int _maxRetry;
        EventLoopPtr _loop;
        ConnectionPtr _connection;
        ConnectorPtr _connector;

        Handle_t _onConnectedCallback;
        ErrorHandle_t _onConnectErrorCallBack;
        ReadHandle_t _onReadCallback;
        CloseHandle_t _onCloseCallback;
        ExceptHandle_t _onExceptCallback;
    };
};

#endif //KQEVENT_TCPCLIENT_H
