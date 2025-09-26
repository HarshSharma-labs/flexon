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

#define POINTER_EVENT_QUEUE_SIZE 4
#define POINTER_EVENT_SUBMIT_SIZE 2
#define KEYBOARD_EVENT_QUEUE_SIZE 4
#define KEYBOARD_EVENT_SUBMIT_SIZE 2
#define POINTER_VIEW_STARTUP_SIZE 10
#define POINTER_EVENT_DISPATCH_THRESH 10
#define POINTER_FUNC_SIZE 10
#define SIC static inline
#define SCE static constexpr
/*
*  event queue cyclic type:
*  
*  _________________________________________________
* |                     | submitted queue to the   |
* | current in progress | dispatcher to process    |
* |_____________________|__________________________|
*  0                    2                          4
* 
* and vice-versa cycle repeats;
*/


class statemanager{
public:
  statemanager(); 
 
   void dispatchEvents();
  // void stackPointerEvent();
// void dispatchKeyboardEvent();
// void dispatchwip();
// void fibertorender();

 static pointer_event* getNextPointerQueue();
 static key_event* getNextKeyboardQueue();

private:

SCE int pointereventsize64 = sizeof(struct pointer_event) / 
                                          sizeof(uint64_t);

SCE int keyeventsize64 = sizeof(key_event) / sizeof(uint64_t);

SIC int cyclepointer = 0;
SIC int cycleKeyboard = 0;
SIC int stridepointer = 0;
SIC bool togglepointer = false;
SIC bool togglekeyboard = false;

SIC sem_t syncstatemanager;
SIC struct pointer_event *Pevent;
SIC struct pointer_event queuePointer[POINTER_EVENT_QUEUE_SIZE];
SIC struct key_event *Kevent;
SIC struct key_event queueKeyboard[KEYBOARD_EVENT_QUEUE_SIZE];

SIC sem_t signaldispatchEvent;
//SIC void(*onpress)()[POINTER_FUNC_SIZE];

SIC std::unordered_map<uint32_t,fiber *> onpressCallback;
SIC std::unordered_map<uint32_t,fiber *> onlongpressCallback;
SIC std::unordered_map<uint32_t,fiber *> keyboardCallback;
SIC std::unordered_map<uint32_t,fiber *> fiberStates;

//SIC std::vector<struct wrapperPointerCallback> pointerEventViews;

};

#endif

