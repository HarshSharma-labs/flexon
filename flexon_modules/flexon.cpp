#include "./subsystem/layoutmanager/layout.hpp"
#include "./subsystem/statemanager/statemanager.hpp"
#include "./flexon.hpp"



void flexon::start(struct commit_wm *commit){
  sem_wait(&statemanager::syncstartupback);
  sem_wait(&statemanager::syncstartup);
  initial_commit(flexon::app_main,flexon::post_startup,commit);
};


void flexon::dispatchCallback(){
 

 label_continue_dispatch:
    sem_wait(&statemanager::dispatchCallbackExt);
    statemanager::submiteddispatchCallback
    [statemanager::submitdispatchfomindex].call(nullptr);
  goto label_continue_dispatch;
};

void flexon::exit(){

};
