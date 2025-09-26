#include "./statemanager.hpp"
#include "../../utilities/utility.hpp"
#include "../windowManager/wayland-callback.hpp"

statemanager::statemanager(){
  Pevent = queuePointer;
  Kevent = queueKeyboard;
  sem_init(&signaldispatchEvent,0,1);
  sem_init(&syncstatemanager,0,1);
  sem_wait(&syncstatemanager);

};

void statemanager::dispatchEvents(){

struct pointer_event queuePSubmit;
struct key_event queueKSubmit;

jmp1:
sem_wait(&syncstatemanager);
    utility::strings::memcpy64(Pevent,&queuePSubmit,pointereventsize64);
    utility::strings::memcpy64(Kevent,&queueKSubmit,keyeventsize64);
   // stackPointerEvent();

  goto jmp1;

};



pointer_event* statemanager::getNextPointerQueue(){
    if(!togglepointer)
       return queuePointer;

   sem_post(&syncstatemanager);
   cyclepointer += 1;
   bool check = (cyclepointer < POINTER_EVENT_QUEUE_SIZE); 
   cyclepointer *= check;
   Pevent = queuePointer + cyclepointer; 
   return Pevent;
 };

key_event* statemanager::getNextKeyboardQueue(){

   if(!togglekeyboard)
      return queueKeyboard;

   sem_post(&syncstatemanager);
   cycleKeyboard += 1;
   bool check = (cycleKeyboard < KEYBOARD_EVENT_SUBMIT_SIZE);
   cycleKeyboard *= check;
   Kevent = queueKeyboard + cycleKeyboard; 
   return Kevent;
};


