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

namespace KQEvent {
    class Connection : public std::enable_shared_from_this<Connection> {
    public:
        using ConnectionPtr = std::shared_ptr<Connection>;
        using Command_t = Observer::Command_t;
        using Handle_t = std::function<Command_t(ConnectionPtr)>;
        /*当Acceptor新建一个连接的时候，设置为connecting状态。而后Acceptor会将该连接对象
         *交给TCPServer,当TCPServer设置好connection的状态之后(例如设置上下文，设置回调等等)
         *就可以开始将connection放入事件循环中使用，但在此之前需要将connection的状态设置为connected
         * */

        /*connection的断开连接有三种方式方式：
         * 1. 调用softClose, 等待缓冲区内数据发送完毕之后关闭连接。
         * 2. 调用forceClose, 立即关闭连接。
         * 3. connection生命周期结束，立即关闭连接。
         * 注意： 关闭连接不意味着connection的生命周期结束，至于connection生命周期何时结束，资源何时
         * 被回收，取决于connection的持有者 -- 如TCPServer
         * */
        typedef enum{Connecting, Connected, Disconnecting, Disconnected}StateE;

        Connection(Connection const &) = delete;

        Connection const &operator=(Connection const &) = delete;

        virtual ~Connection();//需要管理连接的生命周期

        template<typename ..._Args>
        static ConnectionPtr newInstance(_Args &&...args) {
            auto aNew = new Connection(std::forward<_Args>(args)...);
            return ConnectionPtr(aNew);
        }


        ConnectionPtr getPtr(void) {
            return shared_from_this();
        }

        void attachReadHandler(Handle_t handle);

        void attachExceptHandler(Handle_t handle);

        size_t sendMessage(char const *buf, size_t len);

        Subject::SubjectPtr const &getSubject() {
            return _subject;
        }

        void setConnected();

        void setConnecting();

        void setDisconnected();

        void setDisconnecting();

        StateE getStatus();

        int getFd() { return _sockfd; }

        size_t getBufferSize() { return _bufSize; }
        void setBufferSize(size_t size){_bufSize = size;}

        char const *getBuffer() { return _buf; }

        void *getContext(){return _context;}

        void setContext(void *context){_context = context;}

        IPAddress::IPAddressPtr getHostAddr(){return _hostAddress;}

        IPAddress::IPAddressPtr getPeerAddr(){return _peerAddress;}

        //等待缓冲区内数据发送完毕再关闭连接
        void softClose(std::function<void(ConnectionPtr)> cb);

    private:
        explicit Connection(Socket::SocketPtr socket, IPAddress::IPAddressPtr peer, void *context = nullptr);

        static Command_t writeHandler(ConnectionPtr);

        int _sockfd;
        Socket::SocketPtr _socket;

        IPAddress::IPAddressPtr _hostAddress;

        IPAddress::IPAddressPtr _peerAddress;

        Subject::SubjectPtr _subject;

        Observer::ObserverPtr _writeObserver;
        std::vector<Observer::ObserverPtr> _readObservers;
        std::vector<Observer::ObserverPtr> _exceptObservers;

        TCPInfo::TCPInfoPtr _info;

        Observer::Handle_t wrap(Handle_t handle);

        Command_t _handleWrapper(Handle_t handle, Subject::SubjectPtr subject);

        StateE _state;

        bool _softClose;

        std::function<void(ConnectionPtr)> _softCloseCallBack;

        char *_buf;      //fixme : Buffer class;

        size_t _bufSize;

        void *_context; //fixme : Context class;
    };
}

#endif //KQEVENT_CONNECTION_H
