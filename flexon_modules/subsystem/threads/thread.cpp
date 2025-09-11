#include "../windowManager/wayland.hpp"
#include "./thread.hpp"
#include <iostream>
#include <pthread.h>


//static void __renderer__wrapper();
//static void __app_wrapper();
//static void __state_wrapper();
static void *__window_manager_wrapper(void *args);

void __call__thread__subsystem(){
  pthread_attr_t thread_attr;
  pthread_t id;

  pthread_attr_init(&thread_attr);
  int __app_process = pthread_create(&id,&thread_attr,__window_manager_wrapper,NULL); 

  pthread_attr_destroy(&thread_attr);
  //invoke app maker

   pthread_setname_np(id, "window");
   
    // Wait for the created thread to finish
    pthread_join(id, NULL);
    

  //invoke state
 // int __state_process = pthread_create();

  //invoke renderer
  
  //wait for all thread to finish and then exit
};


static void *__window_manager_wrapper(void *args){
 waylandWM window_manager;
  
   window_manager.thread_hello();
  
return nullptr; 
};
