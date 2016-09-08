#include <functional>
#include <atomic>

class AbsSubject;
class AbsObserver{
public:
    typedef enum{DELETE, ALIVE}Command_t;
    using Handle_t = std::function<Command_t(AbsSubject *)>; 
	
    AbsObserver(Handle_t const &handle):_handle(handle){}
    AbsObserver(AbsObserver const&) = delete;
    AbsObserver &operator=(AbsObserver const &) = delete;
    
    void setHandle(Handle_t const &handle){
	_handle = handle;
    }

    Handle_t getHandle(){return _handle;}

    virtual Command_t Update(AbsSubject *subject){
	return _handle(subject);
    }

private:
    Handle_t _handle;
};
