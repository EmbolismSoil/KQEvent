//
// Created by lee on 16-9-15.
//

#include "Connection.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include "IOMessage.h"
#include "SendfileMessage.h"

namespace KQEvent {

    Connection::Connection(Socket::SocketPtr socket, IPAddress::IPAddressPtr peer, void *contex) :
            _sockfd(socket->getFd()),
            _peerAddress(peer),
            _subject(Subject::newInstance(socket->getFd())),
            _state(Connection::Disconnected),
            _softClose(false),
            _context(contex) {
        _writeObserver = Observer::newInstance();
        _readObserver = Observer::newInstance();
        _exceptObserver = Observer::newInstance();

        auto readHandler = std::bind(&Connection::_readHandler,
                                     this, std::placeholders::_1);
        auto writeHandler = std::bind(&Connection::_writeHandler,
                                      this, std::placeholders::_1);
        auto exceptHandler = std::bind(&Connection::_exceptHandler,
                                       this, std::placeholders::_1);

        _readObserver->setHandle(readHandler);
        _writeObserver->setHandle(writeHandler);
        _exceptObserver->setHandle(exceptHandler);

        _subject->attachWriteObserver(_writeObserver);
        _subject->attachReadObserver(_readObserver);
        _subject->attachExceptObserver(_exceptObserver);

        _state = Connecting;
        _socket = socket;
        _info = TCPInfo::fromTCPSocketFd(socket->getFd());
        _hostAddress = _socket->getIPAddress();
    }

    Observer::Handle_t Connection::wrap(Connection::Handle_t handle) {
        return std::bind(&Connection::_handleWrapper,
                         this, handle, std::placeholders::_1);
    }

    Observer::Command_t
    Connection::_handleWrapper(Connection::Handle_t handle,
                               Subject::SubjectPtr subject) {
        return handle(getPtr());
    }

    void Connection::attachReadHandler(Connection::ReadHandle_t handle) {
        _readHandlerCallback = handle;
    }

    void Connection::attachExceptHandler(ExceptHandle_t handle) {
        _exceptCallback = handle;
    }

    Observer::Command_t Connection::_writeHandler(Subject::SubjectPtr subject) {
        if (_messages.empty()){
            subject->setWriteEvent(false);
            return Observer::ALIVE;
        }

        for (auto msgPos = _messages.begin(); msgPos != _messages.end(); ){
            auto msg = *msgPos;
            int status = msg->send(getFd());

            if (status == AbstractMessage::CONTINUE){
                break;
            }else{
                msgPos = _messages.erase(msgPos);
            }
        }

        if (_messages.empty()){
            subject->setWriteEvent(false);
            if (_softClose){
                setDisconnected();
                _socket.reset();
                _closeHandlerCallback(getPtr());
            }
        }

        return Observer::ALIVE;
    }

    void Connection::sendMessage(char const *buf, size_t len) {
        auto msg = IOMessage::newInstance(buf, len);
        _messages.push_back(msg);
        _subject->setWriteEvent(true);
        //如果不在事件循环中则需要唤醒
    }

    void Connection::setConnected() {
        _state = Connected;
    }

    void Connection::setConnecting() {
        _state = Connecting;
    }

    void Connection::setDisconnected() {
        _state = Disconnected;
    }

    void Connection::setDisconnecting() {
        _state = Disconnecting;
    }

    Connection::StateE Connection::getStatus() {
        return _state;
    }

    void Connection::softClose() {
        _softClose = true;
    }

    Connection::Command_t Connection::_readHandler(Subject::SubjectPtr subject) {
        char buf[32768];
        int n = ::read(getFd(), buf, sizeof(buf));
        if (n == 0) {
            _closeHandlerCallback(getPtr());
        } else if (n > 0) {
            _readHandlerCallback(getPtr(), buf, n);
        }
        
        /*
            这里需要处理ECONNECTIONREST的错误，这个错误
            是由于读时对方已经关闭连接造成的，此时读取socket
            会返回ECONNECTIONREST错误
        */
        return Observer::ALIVE; //出现错误
    }

    Connection::Command_t
    Connection::_exceptHandler(Subject::SubjectPtr) {
        _exceptCallback(getPtr());
        return Observer::ALIVE;
    }

    bool Connection::sendFile(const std::string &path) {
        int fd = ::open(path.c_str(), O_RDONLY);
        if (fd < 0)
            return false;

        auto sendfileMsg = SendfileMessage::newInstance(fd);
        _messages.push_back(sendfileMsg);
        _subject->setWriteEvent(true);

        return true;
    }

    void Connection::sendMessage(std::string &&msg) {
        auto m = IOMessage::newInstance(msg);
        _messages.push_back(m);
        _subject->setWriteEvent(true);
    }

    void Connection::sendMessage(std::string const &msg) {
        auto m = IOMessage::newInstance(msg);
        _messages.push_back(m);
        _subject->setWriteEvent(true);
    }

}
