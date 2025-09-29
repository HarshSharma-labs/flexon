#include "../windowManager/wayland.hpp"
#include "../../components/View.hpp"
#include "../layoutmanager/layout.hpp"
#include "../flexon_commits.hpp"
#include "../renderer/renderer.hpp"
#include "../../flexon.hpp"
#include "../statemanager/statemanager.hpp"
#include "./thread.hpp"
#include <pthread.h>
#include <iostream>

 struct commit_wm commitwm;

 

static void *__window_manager_wrapper(void *args){
  sem_wait(&statemanager::syncstartup);

  struct commit_wm *commit = (struct commit_wm*)args; 
  waylandWM window_manager;
  int code = window_manager.create_commit(commit);

  if(code != 0)
      return nullptr;

   sem_post(&statemanager::syncstartup);
   
   sem_wait(&statemanager::syncstartupback);

   window_manager.dispatchEvent(commit);
   sem_destroy(&statemanager::syncstartup);
   sem_destroy(&statemanager::syncstartupback);

   return nullptr; 
};

static void* __renderer__wrapper(void *args){
  renderSubsystem drawer;
  drawer.initlise(); 
  drawer.setExtents(commitwm.rc.pixels , {500,500});
  drawer.destroy();
 
 return nullptr;
};

static void* __state_wrapper__(void *args){
 statemanager manager;
 manager.dispatchEvents();
 return nullptr;
};

static void* __app_wrapper__(void *args){
  flexon::start(&commitwm);
  flexon::dispatchCallback();
  flexon::exit();    
  return nullptr;

};

void __call__thread__subsystem(){
 
  statemanager::statemanagerinit();

  pthread_attr_t thread_attr;
  pthread_t id[4];
 
  commitwm.name = (char *)"hello";
  pthread_attr_init(&thread_attr);


  /*
  if (sem_init(&commitwm.signal, 0, 1) == -1) {
        perror("sem_init");
        exit(EXIT_FAILURE);
   }
  */
    int __state_process = pthread_create(&id[1],&thread_attr,__state_wrapper__,&commitwm); 
    int __wm_process = pthread_create(&id[0],&thread_attr,__window_manager_wrapper,&commitwm); 
    int __app_process = pthread_create(&id[2],&thread_attr,__app_wrapper__,&commitwm);
    int __renderer_process = pthread_create(&id[3],&thread_attr,__renderer__wrapper,nullptr);

  pthread_attr_destroy(&thread_attr);  
  pthread_join(id[0], NULL);
    
};
