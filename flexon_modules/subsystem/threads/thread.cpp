#include "../windowManager/wayland.hpp"
#include "../flexon_commits.hpp"
#include "./thread.hpp"
#include <pthread.h>
#include <iostream>


//static void __app_wrapper();
//static void __state_wrapper();

static void *__window_manager_wrapper(void *args);
static void *__renderer__wrapper(void *args);

void __call__thread__subsystem(){
  pthread_attr_t thread_attr;
  pthread_t id;
  struct commit_wm commitwm;
  commitwm.name = (char *)"hello";

  pthread_attr_init(&thread_attr);
  int __app_process = pthread_create(&id,&thread_attr,__window_manager_wrapper,&commitwm); 
  
  int __renderer_process = 
  pthread_attr_destroy(&thread_attr);  
    pthread_join(id, NULL);
    
};


static void *__window_manager_wrapper(void *args){
   waylandWM window_manager;
   struct commit_wm *commit = (struct commit_wm*)args; 
   window_manager.create_commit(commit);
   window_manager.dispatchEvent(); 
   return nullptr; 
};

static void *__renderer__wrapper(void *args){

};
