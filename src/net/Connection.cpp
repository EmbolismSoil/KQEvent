//
// Created by lee on 16-9-15.
//

#include "Connection.h"
#include <unistd.h>

namespace KQEvent{

    Connection::Connection(int fd, IPAddress::IPAddressPtr peer, void *contex) :
        _sockfd(fd),
        _peerAddress(peer),
        _subject(Subject::newInstance(fd)),
        _info(TCPInfo::fromTCPSocketFd(fd)),
        _bufSize(0),
        _context(contex)
    {
        _buf = new char[32768];
        _writeObserver = Observer::newInstance();
        _writeObserver->setHandle(wrap(writeHandler));
        _subject->attachWriteObserver(_writeObserver);
        _state = Connecting;
        _socket = Socket::newInstance(fd);
        _info = TCPInfo::fromTCPSocketFd(fd);
        _hostAddress = _socket->getIPAddress();
    }

    Observer::Handle_t Connection::wrap(Connection::Handle_t handle) {
        return std::bind(&Connection::_handleWrapper,
                         this, handle, std::placeholders::_1);
    }

    Observer::Command_t
    Connection::_handleWrapper(Connection::Handle_t handle,
                               Subject::SubjectPtr subject)
    {
        return handle(getPtr());
    }

    void Connection::attachReadHandler(Connection::Handle_t handle) {
        auto observer = Observer::newInstance();
        _readObservers.push_back(observer);
        observer->setHandle(wrap(handle));
        _subject->attachReadObserver(observer);
        if (!_subject->getEventMask().READ){
            _subject->setReadEvent(true);
        }
    }

    void Connection::attachExceptHandler(Connection::Handle_t handle) {
        auto observer = Observer::newInstance();
        _exceptObservers.push_back(observer);
        observer->setHandle(wrap(handle));
        _subject->attachExceptObserver(observer);
        if (!_subject->getEventMask().WRITE){
            _subject->setReadEvent(true);
        }
    }

    Observer::Command_t Connection::writeHandler(Connection::ConnectionPtr conn){
        size_t n = 0;
        auto cnt = 0;
        auto buf = conn->getBuffer();
        auto size = conn->getBufferSize();

        while(size && (n = ::write(conn->getFd(), &buf[cnt], size)) > 0){
            size -= n;
            cnt += n;
        }

        conn->setBufferSize(size);

        if (size <= 0){//已经发送完成，不对写事件感兴趣了。
            conn->_subject->setWriteEvent(false);
        }
        return Observer::ALIVE;
    }

    size_t Connection::sendMessage(char const *buf, size_t len) {
        if (_bufSize > 32760)
            return 0;
        int size = 32760 - _bufSize;
        size = size > len ? len : size;
        int cnt = 0;
        for (; cnt < size; ++cnt)
            _buf[_bufSize + cnt] = buf[cnt];
        _bufSize += cnt;
        if (_bufSize > 0)
            _subject->setWriteEvent(true);
        return cnt;
    }

    Connection::~Connection() {

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
}
