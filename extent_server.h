// this is the extent server

#ifndef extent_server_h
#define extent_server_h

#include <string>
#include <map>
#include <set>
#include "extent_protocol.h"
#include <mutex>



class extent_server {

 

 public:
  typedef extent_protocol::extentid_t key_t;
  typedef std::lock_guard<std::mutex>  LockGurad;
  extent_server();

  int put(extent_protocol::extentid_t id, std::string, int &);
  int get(extent_protocol::extentid_t id, std::string &);
  int getattr(extent_protocol::extentid_t id, extent_protocol::attr &);
  int remove(extent_protocol::extentid_t id, int &);
  int genINum( int is_file, key_t & ino);
  int create(key_t parentId, std::string name, int isFile,key_t & ret);
  void addChildren(key_t & parentId,key_t & childId, std::string & name);
  int lookUp(key_t parentId, std::string name, int & isFound ,key_t & childId, extent_protocol::attr & attr);
  private:
  std::map<key_t, std::string> content_;
  std::map<key_t, extent_protocol::attr> attr_;
  std::map<key_t,key_t> parent_;
  std::map<key_t,std::vector<key_t>> children_;
  std::mutex m_;
};

#endif 







