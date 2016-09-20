//
// Created by lee on 16-9-15.
//

#ifndef KQEVENT_CONNECTION_H
#define KQEVENT_CONNECTION_H

#include <memory>
#include "IPAddress.h"
#include "../base/Subject.h"
#include "TCPInfo.h"
#include "../base/Observer.h"
#include "Socket.h"
#include <list>
#include "AbstractMessage.h"

namespace KQEvent {
    class Connection : public std::enable_shared_from_this<Connection> {
    public:
        using ConnectionPtr = std::shared_ptr<Connection>;
        using Command_t = Observer::Command_t;
        using Handle_t = std::function<Command_t(ConnectionPtr)>;
        using ReadHandle_t = std::function<void(ConnectionPtr, char *, size_t)>;
        using CloseHandle_t = std::function<void(ConnectionPtr)>;
        using ExceptHandle_t = CloseHandle_t;
        using MessagePtr = AbstractMessage::AbstractMessagePtr;
        /*当Acceptor新建一个连接的时候，设置为connecting状态。而后Acceptor会将该连接对象
         *交给TCPServer,当TCPServer设置好connection的状态之后(例如设置上下文，设置回调等等)
         *就可以开始将connection放入事件循环中使用，但在此之前需要将connection的状态设置为connected
         * */

        /*connection的断开连接有三种方式方式：
         * 1. 调用softClose, 等待缓冲区内数据发送完毕之后关闭连接。
         * 2. 调用forceClose, 立即关闭连接。  //目前尚未实现forceClose,因为方式3可以替代之
         * 3. connection生命周期结束，立即关闭连接。
         * 对端关闭时，connection会回调_closeHandlerCallback，这里是用户回收connection的好时机
         * 注意： 关闭连接不意味着connection的生命周期结束，至于connection生命周期何时结束，资源何时
         * 被回收，取决于connection的持有者 -- 如TCPServer
         * */
        typedef enum {
            Connecting, Connected, Disconnecting, Disconnected
        } StateE;

        Connection(Connection const &) = delete;

        Connection const &operator=(Connection const &) = delete;

        template<typename ..._Args>
        static ConnectionPtr newInstance(_Args &&...args) {
            auto aNew = new Connection(std::forward<_Args>(args)...);
            return ConnectionPtr(aNew);
        }


        ConnectionPtr getPtr(void) {
            return shared_from_this();
        }

        void attachReadHandler(ReadHandle_t);

        void attachExceptHandler(ExceptHandle_t handle);

        void attachCloseHandler(CloseHandle_t handler) {
            _closeHandlerCallback = handler;
        }

        void sendMessage(char const *buf, size_t len);
        void sendMessage(std::string&& msg);
        void sendMessage(std::string const& msg);

        Subject::SubjectPtr const &getSubject() {
            return _subject;
        }

        void setConnected();

        void setConnecting();

        void setDisconnected();

        void setDisconnecting();

        StateE getStatus();

        int getFd() { return _sockfd; }

        void *getContext() { return _context; }

        void setContext(void *context) { _context = context; }

        IPAddress::IPAddressPtr getHostAddr() { return _hostAddress; }

        IPAddress::IPAddressPtr getPeerAddr() { return _peerAddress; }

        //等待缓冲区内数据发送完毕再关闭连接
        void softClose();

        bool sendFile(std::string const &path);

    private:
        explicit Connection(Socket::SocketPtr socket, IPAddress::IPAddressPtr peer, void *context = nullptr);

        Command_t _writeHandler(Subject::SubjectPtr);

        Command_t _readHandler(Subject::SubjectPtr);

        Command_t _exceptHandler(Subject::SubjectPtr);

        int _sockfd;
        Socket::SocketPtr _socket;

        IPAddress::IPAddressPtr _hostAddress;

        IPAddress::IPAddressPtr _peerAddress;
        Subject::SubjectPtr _subject;

        Observer::ObserverPtr _writeObserver;
        Observer::ObserverPtr _readObserver;
        Observer::ObserverPtr _exceptObserver;

        ReadHandle_t _readHandlerCallback;
        CloseHandle_t _closeHandlerCallback;
        ExceptHandle_t _exceptCallback;

        TCPInfo::TCPInfoPtr _info;

        Observer::Handle_t wrap(Handle_t handle);

        Command_t _handleWrapper(Handle_t handle, Subject::SubjectPtr subject);

        StateE _state;

        bool _softClose;

        void *_context; //fixme : Context class;

        std::list<MessagePtr> _messages;
    };
}

#endif //KQEVENT_CONNECTION_H
