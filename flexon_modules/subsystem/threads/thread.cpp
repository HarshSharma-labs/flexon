#include "../windowManager/wayland.hpp"
#include "../flexon_commits.hpp"
#include "./thread.hpp"
#include <pthread.h>
#include <iostream>




static void *__window_manager_wrapper(void *args){
   waylandWM window_manager;
   struct commit_wm *commit = (struct commit_wm*)args; 
   window_manager.create_commit(commit);
   window_manager.dispatchEvent(); 
   return nullptr; 
};

static void *__renderer__wrapper(void *args){
 return nullptr;
};

static void __state_wrapper__(void *args){

};

void __call__thread__subsystem(){
  pthread_attr_t thread_attr;
  pthread_t id[2];
  struct commit_wm commitwm;
  commitwm.name = (char *)"hello";

  pthread_attr_init(&thread_attr);
  int __app_process = pthread_create(&id[0],&thread_attr,__window_manager_wrapper,&commitwm); 
  
//  int __state_process = pthread_create(&id[1],&thread_attr,__state_wrapper__,&commitwm);

  pthread_attr_destroy(&thread_attr);  
  pthread_join(id[0], NULL);
    
};


