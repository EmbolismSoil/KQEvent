//
// Created by lee on 16-9-15.
//

#include "Connection.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>


namespace KQEvent{

    Connection::Connection(Socket::SocketPtr socket, IPAddress::IPAddressPtr peer, void *contex) :
        _sockfd(socket->getFd()),
        _peerAddress(peer),
        _subject(Subject::newInstance(socket->getFd())),
        _state(Connection::Disconnected),
        _softClose(false),
        _bufSize(0),
        _context(contex)
    {
        _buf = new char[32768];
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
                               Subject::SubjectPtr subject)
    {
        return handle(getPtr());
    }

    void Connection::attachReadHandler(Connection::ReadHandle_t handle) {
        _readHandlerCallback = handle;
    }

    void Connection::attachExceptHandler(ExceptHandle_t handle) {
        _exceptCallback = handle;
    }

    Observer::Command_t Connection::_writeHandler(Subject::SubjectPtr subject){
        //优先发送消息，但是如果在sendfile尚未完成时，是不能发送消息的。
        int n = 0;
        auto cnt = 0;
        auto buf = getBuffer();
        auto size = getBufferSize();

        if (_isSendfile){
            __doSendfile();
            return Observer::ALIVE;
        }

        while(size && (n = ::write(getFd(), &buf[cnt], size)) > 0){
            size -= n;
            cnt += n;
        }

        setBufferSize(size);

        if (size <= 0){
            if (_sendfileQueue.empty()){//已经发送完成，不对写事件感兴趣了。
                _subject->setWriteEvent(false);
                if (_softClose){ //是否需要关闭？
                    setDisconnected();
                    _socket.reset();//socket生命周期结束
                    _closeHandlerCallback(getPtr());//这里已经不能再发送网络消息了
                }
            }else{
                _isSendfile = true;
                __doSendfile();
            }
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
        delete[] _buf;
        //这里不用关闭fd,而是由socket作为一个RAII类关闭连接
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

    Connection::Command_t Connection::_readHandler(Subject::SubjectPtr subject){
        char buf[32768];
        int n = ::read(getFd(), buf, sizeof(buf));
        if (n == 0){
            _closeHandlerCallback(getPtr());
        }else if (n > 0){
            _readHandlerCallback(getPtr(), buf, n);
        }

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

        struct stat statbuff;
        if(::fstat(fd, &statbuff) < 0){
            return false;
        }
        sendFileDesc desc = {.fd = fd, .size = statbuff.st_size};
        _sendfileQueue.push_back(desc);
        _subject->setWriteEvent(true);
        return true;
    }

    void Connection::__doSendfile() {
        _isSendfile = true;
        auto pos = _sendfileQueue.begin();
        int n = 0;
        while((n = ::sendfile(getFd(), pos->fd, NULL, pos->size)) > 0){
            pos->size -= n;
        }

        if (n == EAGAIN || n == 0) {
            //正常结束
            if (pos->size <= 0){
                _isSendfile = false;
                _sendfileQueue.erase(pos);
            }
            return;
        }

        if (n < 0){
            //不正常结束
            _exceptCallback(getPtr());
            _isSendfile = false;
            _sendfileQueue.erase(pos);
            return;
        }
    }
}
