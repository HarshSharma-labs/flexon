#ifndef __FLEXON_STATEMAMAGER__
#define __FLEXON_STATEMANAGER__

/*
 * WARNING UNOPTIMISED-CODE
 *
 */

#include <vector>
#include <unordered_map>
#include <semaphore.h>
#include <cstdint>
#include <iostream>
#include "../windowManager/wayland-callback.hpp"
#include "../../components/base.hpp"

#define EVENT_QUEUE_SIZE 10
#define EVENT_QUEUE_HALF_SIZE 2
#define POINTER_VIEW_STARTUP_SIZE 10
#define POINTER_EVENT_DISPATCH_THRESH 10
#define DEFAULT_PRESS_QUEUE_SIZE 10

#define SIC static inline
#define SCE static constexpr


class statemanager{
public:
  statemanager(); 
 
   void dispatchEvents();
   static void registerEvents(struct event_wrapper *event , int index);
   void stackPointerEvent(struct pointer_event &event,float &px,
                           float &py);
   void flushKeyboardBuffer(struct key_event &event);
// void dispatchKeyboardEvent();
// void dispatchwip();
// void fibertorender();

 static pointer_event* getNextPointerQueue();
 static key_event* getNextKeyboardQueue();

 //must be called first;
 static void init(){
   statemanager::pointerEventViews.reserve(DEFAULT_PRESS_QUEUE_SIZE);
  };
 static void registerEventCallback(fiber *which , callbackfunction callback){
    statemanager::togglepointer = true;
    statemanager::pressCallback[which->fiberid] = callback;
    struct fiber_event_wrapper tmp = {.fiber_id = which->fiberid, 
                                     .geometry = &which->paint->geometry };

    statemanager::pointerEventViews.push_back(tmp);
  };
  static std::vector<fiber_event_wrapper> &returnViewData(){
   return pointerEventViews; 
  };  

private:

SCE int KeventSize64 = sizeof(struct key_event) / sizeof(uint64_t);
SCE int PeventSize64 = sizeof(struct pointer_event) / sizeof(uint64_t);
SCE int eventWrapperSize64 = sizeof(struct event_wrapper) / sizeof(uint64_t);

SIC int cyclepointer = 0;
SIC int cycleKeyboard = 0;

SIC bool togglepointer = false;
SIC bool togglekeyboard = false;
SIC bool EventContinue = false;
SIC sem_t syncstatemanager;
SIC sem_t syncback;
SIC struct event_wrapper *Pevent;
SIC struct event_wrapper eventQueue[EVENT_QUEUE_SIZE];

SIC sem_t signaldispatchEvent;
SIC int dispatchpointerfrom = 0;
SIC std::unordered_map<uint32_t,callbackfunction> pressCallback;
//SIC std::unordered_map<uint32_t,fiber *> keyboardCallback;
//SIC std::unordered_map<uint32_t,fiber *> fiberStates;

SIC std::vector<fiber_event_wrapper> pointerEventViews;

};

#endif

