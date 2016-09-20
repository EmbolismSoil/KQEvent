//
// Created by lee on 16-9-20.
//

#ifndef KQEVENT_ABSTRACTMESSAGE_H
#define KQEVENT_ABSTRACTMESSAGE_H

#include <memory>

namespace KQEvent {
    class AbstractMessage {
        /*不要被骗了，这其实更像一个interface而不是class, 因缺思厅~*/
    public:
        /* CONTINUE 表示该消息还没发完，需要继续发送。
         * FINISHED 表示消息发送完毕。
         * ERROR 表示发送错误，可以通过getErrorCode获取错误状态码
         * */
        typedef enum {
            CONTINUE = 0, FINISHED, ERROR
        } statusE;
        using AbstractMessagePtr = std::shared_ptr<AbstractMessage>;

        virtual ~AbstractMessage(){};

        virtual statusE send(int fd) = 0;

        virtual int getErrorCode() = 0;
    };
}


#endif //KQEVENT_ABSTRACTMESSAGE_H
