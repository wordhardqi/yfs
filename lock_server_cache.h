#ifndef lock_server_cache_h
#define lock_server_cache_h

#include <string>

#include <map>
#include "lock_protocol.h"
#include "rpc.h"
#include "lock_server.h"
#include <muduo/base/Mutex.h>
struct lock_cache_state{
    enum STATE{
        FREE = 0,
        BUSY
    } state;
    lock_cache_state() = default;
    std::string owner;
};

class lock_server_cache {
 private:
  int nacquire;
  std::map<lock_protocol::lockid_t , lock_cache_state> locks_;
 public:
  lock_server_cache();
  lock_protocol::status stat(lock_protocol::lockid_t, int &);
  int acquire(lock_protocol::lockid_t, std::string id, int &);
  int release(lock_protocol::lockid_t, std::string id, int &);
  muduo::MutexLock mutex_;
};

#endif
