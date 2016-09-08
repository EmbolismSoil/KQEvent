#include "AbsObserver.h"
#include <memory>
#include <hash_map>
#include <ext/hash_map>

class AbsSubject : std::enable_shared_from_this<AbsSubject>{
public:
    AbsSubject(AbsSubject const &) = delete;
    AbsSubject const &operator=(AbsSubject const&) = delete;

    std::shared_ptr<AbsSubject> getPtr(void){
	return this->shared_from_this();
    }   

    virtual bool attach(std::shared_ptr<AbsObserver> observer){
    	std::weak_ptr<AbsObserver> ptr = observer;
	_observers[observer.get()] = ptr;
	observer->onAttach();
	return true;
    }

    virtual bool detach(std::shared_ptr<AbsObserver> observer){
	_observers.erase(observer.get());
	observer->onDetach();
	return true;
    }
    
    virtual bool notify(void){
	bool ret = true;
	for (auto pos = _observers.begin(); 
		pos != _observers.end();){
            auto tmp = pos;
	    auto ptr = pos->second;
	    if (auto observer = ptr.lock()){
		if( observer->update(shared_from_this()) == AbsObserver::DELETE ){
		    ++pos;
		    _observers.erase(tmp);
		}else{
		    ++pos;
		}
	    }else{
		ret = false;
		++pos;
		_observers.erase(tmp);			
	    }
	}
	return ret;
    }

private:
   __gnu_cxx::hash_map<AbsObserver*, std::weak_ptr<AbsObserver>> _observers;   
};
