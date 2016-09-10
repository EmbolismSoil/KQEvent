//
// Created by lee on 16-9-11.
//
#include "AbsObserver.h"
#include "AbsSubject.h"
#include <iostream>

int main(void) {
    auto handler = [](AbsSubject::AbsSubjectPtr subject){return AbsObserver::DELETE;};
    auto observer = AbsObserver::newInstance(handler,std::string("test"));
    auto handle = observer->getHandle();
    handle(std::make_shared<AbsSubject>());

    return 0;
}
