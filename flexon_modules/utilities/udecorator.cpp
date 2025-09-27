#include "./utility.hpp"
#include <cstdint>
#include <iostream>

bool utility::decorator::checkZero(uint8_t __vsrc ,uint32_t stride){


  for(int i = 0 ; i < 4 ; i++){
   if(__vsrc&(1 << (i + stride))) return false;
  };

  return true;
};

bool utility::decorator::emptyEntity(void *which , size_t size){
  uint64_t *ptr = (uint64_t*)which;
  for(size_t itr = 0 ; itr < size ; itr++)
     if(ptr[itr] != 0) return false;
 
      return true;
};

void utility::decorator::setbits(uint8_t *__vsrc ,float *param, uint32_t stride){

  for(int i = 0 ; i < 4; i++){
    if(param[i] != 0.0f){
      *__vsrc |= 1 << (i + stride);
    };
  }
};
