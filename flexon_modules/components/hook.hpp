#ifndef __FLEXON_HOOKS__
#define __FLEXON_HOOKS__
#include <cstdint>
#include <utility>
#include <map>




class state{

 
};

class hook{

public:
 void addhook(uint32_t fiberid,void *which){
  hooks[fiberid] = which;
 };

 void* gethook(uint32_t fiberid){
   return hooks[fiberid];
 }

private:

  std::map<uint32_t, void*> hooks;
};

#endif
