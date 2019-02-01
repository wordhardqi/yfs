// extent client interface.

#ifndef extent_client_h
#define extent_client_h

#include <string>
#include "extent_protocol.h"
#include "rpc.h"

class extent_client {
public:
 typedef unsigned long long key_t;

 private:
  rpcc *cl;

 public:
  extent_client(std::string dst);

  extent_protocol::status get(extent_protocol::extentid_t eid, 
			      std::string &buf);
  extent_protocol::status getattr(extent_protocol::extentid_t eid, 
				  extent_protocol::attr &a);
  extent_protocol::status put(extent_protocol::extentid_t eid, std::string buf);
  extent_protocol::status remove(extent_protocol::extentid_t eid);
  extent_protocol::status create(key_t parentId, std::string name, int isFile,key_t & ret);

  extent_protocol::status genINum(int isFile, key_t & INum ){
    int ret  = cl->call(extent_protocol::genINum,isFile, INum );
    return ret;
  };

  extent_protocol::status lookUp(key_t parentId, std::string name,int & isFound, key_t & childId, extent_protocol::attr & attr ){
      int ret = cl->call(extent_protocol::lookUp, parentId,name, isFound,childId, attr );
      return ret; 
  };



};

#endif 

