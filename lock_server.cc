// the lock server implementation

#include "lock_server.h"
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

lock_server::lock_server():
  nacquire (0)
{
}

lock_protocol::status
lock_server::stat(int clt, lock_protocol::lockid_t lid, int &r)
{
  lock_protocol::status ret = lock_protocol::OK;
  printf("stat request from clt %d\n", clt);
  r = nacquire;
  return ret;
}
lock_protocol::status
lock_server::acquire(int clt, lock_protocol::lockid_t lid, int &r){
  std::unique_lock<std::mutex> lk(serverMutex_);
  if(locks_.find(lid)==locks_.end()){
      locks_[lid].state = lock_state::BUSY;

  }else{
      while(locks_[lid].state ==lock_state::BUSY){
        locks_[lid].cv.wait(lk);
      }
      locks_[lid].state = lock_state::BUSY;
  }
  return lock_protocol::OK;
}

lock_protocol::status
lock_server::release(int clt, lock_protocol::lockid_t lid, int &r){
  std::unique_lock<std::mutex> lk(serverMutex_);
  if(locks_.find(lid)!=locks_.end() && locks_[lid].state ==lock_state::BUSY){
    locks_[lid].state = lock_state::FREE;
    locks_[lid].cv.notify_all();
  }
  return lock_protocol::OK;
}


