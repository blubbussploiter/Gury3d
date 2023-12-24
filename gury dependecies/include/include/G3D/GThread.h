/** 
  @file GThread.h
 
  @created 2005-09-22
  @edited  2005-10-22

 */

#ifndef G3D_GTHREAD_H
#define G3D_GTHREAD_H

#include "G3D/platform.h"

#include <string>

#ifndef G3D_WIN32
#   include <pthread.h>
#   include <signal.h>
#endif


namespace G3D {

namespace _internal {
    class GThreadPrivate;
}
        

/**
    GThread's documentation
*/
class GThread {
private:

    _internal::GThreadPrivate*          pthread;

    // Thread handle to hold HANDLE and pthread_t
    void*                               handle;

    std::string                         _name;

    // Not implemented on purpose, don't use
    GThread(const GThread &);
    GThread& operator=(const GThread&);
    bool operator==(const GThread&);

public:
    GThread(const std::string& name);
    virtual ~GThread();

    /** Constructs a basic GThread without requiring a subclass.

        @param proc The global or static function for the threadMain() */
    static GThread* create(const std::string& name, void (*proc)());

    /** Starts the thread and executes main() */
    bool start();

    /** Terminates the thread without notifying or
        waiting for a cancelation point. */
    void terminate();

    /**
        Returns true if threadMain is currently executing. */
    bool running();

    /** Returns completed status of thread. */
    bool completed();

    /** 
        Waits for the thread to finish executing. 
        
        TODO: Does this need a timeout? */
    void waitForCompletion();

    /** Returns thread name */
    const std::string& name() {
        return _name;
    }

protected:
    friend class _internal::GThreadPrivate;

    virtual void threadMain() = 0;
};


/**
    GMutex's documentation
*/
class GMutex {
private:
#   ifdef G3D_WIN32
    CRITICAL_SECTION                    handle;
#   else
    pthread_mutex_t                     handle;
#   endif

    // Not implemented on purpose, don't use
    GMutex(const GMutex &mlock);
    GMutex &operator=(const GMutex &);
    bool operator==(const GMutex&);

public:
    GMutex();
    ~GMutex();

    /** Locks the mutex or blocks until available. */
    void lock();

    /** Unlocks the mutex. */
    void unlock();
};


/**
    GMutexLock's documentation.
    Automatically locks while in scope.
*/
class GMutexLock {
private:
    GMutex* m;

    // Not implemented on purpose, don't use
    GMutexLock(const GMutexLock &mlock);
    GMutexLock &operator=(const GMutexLock &);
    bool operator==(const GMutexLock&);

public:
    GMutexLock(GMutex* mutex) {
        m = mutex;
        m->lock();
    }

    ~GMutexLock() {
        m->unlock();
    }
};


} // namespace G3D

#endif //G3D_GTHREAD_H
