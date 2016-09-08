#include <functional>
#include <atomic>
#include <memory>

class AbsSubject;

class AbsObserver : std::enable_shared_from_this<AbsObserver>{
public:
    typedef enum{DELETE, ALIVE}Command_t;
    using Handle_t = std::function<Command_t(std::shared_ptr<AbsSubject>)>; 
	
    AbsObserver(Handle_t const &handle, std::string const &name):
    	_handle(handle), 
	_name(name)
    {

    }

    AbsObserver(AbsObserver const&) = delete;
    AbsObserver &operator=(AbsObserver const &) = delete;
    
    void setHandle(Handle_t const &handle){
	_handle = handle;
    }

    Handle_t getHandle(){return _handle;}

    virtual Command_t update(std::shared_ptr<AbsSubject> subject){
	return _handle(subject);
    }

    std::shared_ptr<AbsObserver> getPtr(void){
	return this->shared_from_this();
    }
    //for log    
    virtual void onAttach(void){}
    virtual void onDetach(void){}

private:
    Handle_t _handle;
    std::string _name;//used by log
};
