 // the extent server implementation

#include "extent_server.h"
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <chrono>
#include <cstdlib>
typedef extent_protocol::extentid_t INUM;

bool
isfile(INUM inum)
{
  if(inum & 0x80000000)
    return true;
  return false;
}

bool
isdir(INUM inum)
{
  return ! isfile(inum);
}
extent_server::extent_server() {}

unsigned int getCurrentTime(){

  return (std::chrono::duration_cast<std::chrono::seconds>(
    std::chrono::system_clock::now().time_since_epoch()
  )).count();
}

int extent_server::put(extent_protocol::extentid_t id, std::string buf, int &ret)
{
  // You fill this in for Lab 2.
  LockGurad lk(m_);
  content_[id] = buf;
  unsigned int curTime = getCurrentTime();
  attr_[id].ctime = curTime;
  attr_[id].mtime = curTime;
  attr_[id].size = content_[id].size();
  ret = attr_[id].size;
  return extent_protocol::OK;
}

int extent_server::get(extent_protocol::extentid_t id, std::string &buf)
{
  // You fill this in for Lab 2.
  LockGurad lk(m_);
  auto it = content_.find(id);
  if(it == content_.end()){
    return extent_protocol::IOERR;
  }
  buf = content_[id];
  attr_[id].atime = getCurrentTime();
  return extent_protocol::OK;
}

int extent_server::getattr(extent_protocol::extentid_t id, extent_protocol::attr &a)
{
  // You fill this in for Lab 2.
  // You replace this with a real implementation. We send a phony response
  // for now because it's difficult to get FUSE to do anything (including
  // unmount) if getattr fails.
  LockGurad lk(m_);
  auto it = attr_.find(id);
  if(it == attr_.end()){
    return extent_protocol::OK;
  }
  a.size = it->second.size;
  a.atime = it->second.atime;
  a.mtime = it->second.mtime;
  a.ctime = it->second.ctime;
  return extent_protocol::OK;
}

int extent_server::remove(extent_protocol::extentid_t id, int &)
{
  content_.erase(id);
  return extent_protocol::OK;
}
int extent_server::genINum( int is_file, INUM & INum){
  INUM  ino = 0;
  std::srand(getCurrentTime());
  do {
    ino = rand() % 0x80000000LLU;
    if(ino ==1) continue;
    if(is_file){
        ino |= 0x80000000LLU;
    }
  } while(content_.find(ino)!= content_.end());
  INum = ino;
  return extent_protocol::OK;
}

void extent_server::addChildren(INUM & parentId,INUM & childId,std::string & name){
  children_[parentId].push_back(childId);
  parent_[childId] = parentId;
  int ret = 0;
  put(parentId,
  content_[parentId] + ":" + name + std::to_string(childId), ret);

}

int extent_server::create(INUM parentId, std::string name, int isFile,INUM & ret){
  LockGurad lk(m_);
  const auto iter = content_.find(parentId);
  if (iter == content_.end()){
    return extent_protocol::NOENT;
  }
  std::vector<INUM> & children = children_[parentId];
  for( auto & child : children){
      if (isfile(child)){
        if (content_[child] == name){
          return extent_protocol::EXIST;
        }
      } 
  }
  INUM ino;
  genINum( isFile, ino);
  int useless_ret;
  put(ino,name,useless_ret);
  ret = ino;
  addChildren(parentId, ino,name);
  return extent_protocol::OK;


  
}




int extent_server::lookUp(INUM parentId, std::string name, int & isFound ,INUM & childId, extent_protocol::attr & attr){
  LockGurad lk(m_);
  isFound = false;
  const auto iter = content_.find(parentId);
  if (iter == content_.end()){
    return extent_protocol::NOENT;
  }
  std::vector<INUM> & children = children_[parentId];
  for (auto & child : children){
    if (content_[child] == name){
      childId = child;
      isFound = true;
      getattr(childId,attr);
      return extent_protocol::OK;
    }
  }
  return extent_protocol::NOENT;

  
}


