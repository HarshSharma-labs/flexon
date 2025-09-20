#include "../windowManager/wayland.hpp"
#include "../../components/View.hpp"
#include "../../utilities/layout.hpp"
#include "../flexon_commits.hpp"
#include "./thread.hpp"
#include <pthread.h>
#include <iostream>




static void *__window_manager_wrapper(void *args){
  struct commit_wm *commit = (struct commit_wm*)args; 
  sem_wait(&commit->signal);

   waylandWM window_manager;
   int code = window_manager.create_commit(commit);
   sem_post(&commit->signal);
   if(code != 0)
      return nullptr;

   window_manager.dispatchEvent();

   return nullptr; 
};

static void* __renderer__wrapper(void *args){
 return nullptr;
};

static void* __state_wrapper__(void *args){
return nullptr;
};

static void* __app_wrapper__(void *args){
  struct commit_wm *commit = (struct commit_wm*)args; 
  //sem_wait(&commit->signal);
  //sem_post(&commit->signal);

  //void *node = app_main();
  
 initial_commit(app_main,post_startup);

  
  return nullptr;

};

void __call__thread__subsystem(){

  pthread_attr_t thread_attr;
  pthread_t id[2];

  struct commit_wm commitwm;
  commitwm.name = (char *)"hello";
  commitwm.width = 500;
  commitwm.height = 500;
  pthread_attr_init(&thread_attr);

  /*
  if (sem_init(&commitwm.signal, 0, 1) == -1) {
        perror("sem_init");
        exit(EXIT_FAILURE);
   }
  */
   
  //int __app_process = pthread_create(&id[0],&thread_attr,__window_manager_wrapper,&commitwm); 
  int __state_process = pthread_create(&id[1],&thread_attr,__app_wrapper__,&commitwm); 
  pthread_attr_destroy(&thread_attr);  
  pthread_join(id[1], NULL);
    
};


