#ifndef yfs_client_h
#define yfs_client_h

#include <string>
//#include "yfs_protocol.h"
#include "extent_client.h"
#include <vector>


class yfs_client {
 public:
  extent_client *ec;

  typedef unsigned long long inum;
  typedef unsigned long long INUM;
  typedef INUM key_t;

  enum xxstatus { OK, RPCERR, NOENT, IOERR, EXIST };
  typedef int status;

  struct fileinfo {
    unsigned long long size;
    unsigned long atime;
    unsigned long mtime;
    unsigned long ctime;
  };
  struct dirinfo {
    unsigned long atime;
    unsigned long mtime;
    unsigned long ctime;
  };
  struct dirent {
    std::string name;
    yfs_client::inum inum;
  };

 private:
  static std::string filename(inum);
  static inum n2i(std::string);
 public:

  yfs_client(std::string, std::string);

  bool isfile(inum);
  bool isdir(inum);

  int getfile(inum, fileinfo &);
  int getdir(inum, dirinfo &);
  yfs_client::status create(key_t parentId, std::string name, int isFile,key_t & ret);
  
  yfs_client::status genINum(int isFile, INUM & INum){
    int ret = ec->genINum(isFile,INum);
    return ret ;
  }
  
  yfs_client::status lookUp(INUM parentId, std::string name,int & isFound, INUM & childId, extent_protocol::attr & attr){
      int ret = ec->lookUp(parentId,name,isFound,childId, attr );
      return ret; 
  };
  
};

#endif 
