#include "./statemanager.hpp"
#include "../../utilities/utility.hpp"
#include "../windowManager/wayland-callback.hpp"
#include "../layoutmanager/layout.hpp"
#include "../../components/base.hpp"

void statemanager::statemanagerinit(){
  Pevent = eventQueue;

  statemanager::pointerEventViews.reserve(DEFAULT_PRESS_QUEUE_SIZE);
  sem_init(&statemanager::syncstartup,0,1);
  sem_init(&statemanager::syncstartupback,0,1);
  sem_init(&statemanager::dispatchCallbackExt,0,1);
  sem_init(&statemanager::signaldispatchEvent,0,1);
  sem_init(&statemanager::syncback,0,1);

  sem_init(&statemanager::syncstatemanager,0,1);
  sem_wait(&statemanager::syncstatemanager);
  sem_wait(&statemanager::dispatchCallbackExt);

};


void statemanager::registerEvents(struct event_wrapper *event , int index){
        utility::strings::memcpy64(event,&statemanager::dispatcherQueue[index],
                               eventWrapperSize64);
  
        sem_post(&syncstatemanager);
 
  return; 
};

void statemanager::dispatchEvents(){
  //don't touch critical section
  int state = -1;
  // are previous x and y location;
  float px = 0.0f ,py = 0.0f;
     
labelDispatchEvent:


  sem_wait(&statemanager::syncstatemanager);
    
  state += 1;
  bool valid = (state < EVENT_QUEUE_SIZE);
  state *= valid;

  switch(dispatcherQueue[state].event_type){
    case EVENT_TYPE_POINTER:
      stackPointerEvent(statemanager::dispatcherQueue[state].epointer
                        ,px,py,state); 
      utility::strings::memset64((statemanager::dispatcherQueue + state),
                                           0,eventWrapperSize64);     
    
    break;
    case EVENT_TYPE_KEYBOARD:
    break;
  }
  
  goto labelDispatchEvent;

};

void statemanager::stackPointerEvent(struct pointer_event &event,float &px , float &py, int state){
  uint32_t trigger = 0;

  switch(event.event_type){
   case WL_POINTER_EVENT_BUTTON:
      trigger = layoutmanager::CheckBound(px , py,pointerEventViews);
      if(trigger != 0){
         auto func = pressCallbacklist[trigger];
         statemanager::submiteddispatchCallback[state].call = func.onpress;
         submitdispatchfomindex = state; 
         sem_post(&statemanager::dispatchCallbackExt); 
        }
   break;
   case WL_POINTER_EVENT_ENTER:
  
    px = event.x;
    py = event.y;

   break;
   case WL_POINTER_EVENT_MOTION:
    px = event.x;
    py = event.y;
   case WL_POINTER_EVENT_LEAVE:
    break;
  };
};

void statemanager::flushKeyboardBuffer(struct key_event &event){
};

pointer_event* statemanager::getNextPointerQueue(){
    if(!togglepointer)
       return &eventQueue->epointer;

   Pevent->event_type = EVENT_TYPE_POINTER;
   registerEvents(Pevent,cyclepointer);

   cyclepointer += 1;
   bool check = (cyclepointer < EVENT_QUEUE_SIZE); 
   cyclepointer *= check;
   Pevent = eventQueue + cyclepointer;
   return &Pevent->epointer;
 };

key_event* statemanager::getNextKeyboardQueue(){

   if(!togglekeyboard)
      return &eventQueue->ekey;

   Pevent->event_type = EVENT_TYPE_KEYBOARD;
   sem_post(&syncstatemanager);
   cycleKeyboard += 1;
   bool check = (cycleKeyboard < EVENT_QUEUE_SIZE);
   cycleKeyboard *= check;
   Pevent = eventQueue + cycleKeyboard; 
   return &Pevent->ekey;
};


