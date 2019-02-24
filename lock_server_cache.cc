// the caching lock server implementation

#include "lock_server_cache.h"
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "lang/verify.h"
#include "handle.h"
#include "tprintf.h"


lock_server_cache::lock_server_cache()
{
}


int lock_server_cache::acquire(lock_protocol::lockid_t lid, std::string id, 
                               int &)
{

  lock_protocol::status ret = lock_protocol::OK;
  muduo::MutexLockGuard lk(mutex_);
  if(locks_.find(lid) == locks_.end()){
    locks_[lid].state = lock_cache_state::BUSY;
    locks_[lid].owner = id;
  }else{
    if (locks_[lid].state == lock_cache_state::BUSY && locks_[lid].owner != id){
      return lock_protocol::RETRY;
    }
  }
  return ret;
}

int 
lock_server_cache::release(lock_protocol::lockid_t lid, std::string id, 
         int &r)
{
  muduo::MutexLockGuard lk(mutex_);
  if(locks_.find(lid)!=locks_.end() && locks_[lid].state ==lock_cache_state::BUSY && locks_[lid].owner == id){
    locks_[lid].state = lock_cache_state::FREE;
  }
  return lock_protocol::OK;
}

lock_protocol::status
lock_server_cache::stat(lock_protocol::lockid_t lid, int &r)
{
  tprintf("stat request\n");
  muduo::MutexLockGuard lk(mutex_);
  r = nacquire;
  tprintf("nacquire %d",r);
  for (auto tlock = locks_.begin(); tlock !=locks_.end(); ++tlock){
    tprintf("lock %d owned by %s is %s ", tlock->first, tlock->second.owner.c_str(),
            tlock->second.state == lock_cache_state::BUSY ? "BUSY":"FREE")
  }
  return lock_protocol::OK;
}

