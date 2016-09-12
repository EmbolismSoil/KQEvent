//
// Created by lee on 16-9-12.
//

#ifndef KQEVENT_COMMON_H
#define KQEVENT_COMMON_H

#include <string>
#include <string.h>

namespace KQEvent{
    size_t  const __ERROR_STR_LEN = 128;
    typedef enum{ERROR, OK}Status_t;
    std::string errno2String(int err){
        char buf[__ERROR_STR_LEN] = {0};
        strerror_r(err, buf, sizeof(buf));
        return std::string(buf);
    }

}

#endif //KQEVENT_COMMON_H
