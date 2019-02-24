// this is the lock server
// the lock client has a similar interface

#ifndef lock_server_h
#define lock_server_h

#include <string>
#include "lock_protocol.h"
#include "lock_client.h"
#include "rpc.h"
#include <map>
#include <condition_variable>
#include <mutex>
#include <functional>

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
TypeName(const TypeName&);                 \
void operator=(const TypeName&)

struct lock_state {
    enum STATE{
      FREE = 0,
      BUSY,
    } state;

    lock_state() = default;
    std::condition_variable cv;
    // DISALLOW_COPY_AND_ASSIGN(lock_state);

};
class lock_server {

 protected:
  int nacquire;
  std::map<lock_protocol::lockid_t,lock_state> locks_;


 public:
  lock_server();
  ~lock_server() {};

  lock_protocol::status stat(int clt, lock_protocol::lockid_t lid, int &);
  lock_protocol::status acquire(int clt, lock_protocol::lockid_t lid, int &r );
  lock_protocol::status release(int clt, lock_protocol::lockid_t lid, int &r );
  
  std::mutex serverMutex_;
  std::condition_variable serverCond_;


};

#endif 







