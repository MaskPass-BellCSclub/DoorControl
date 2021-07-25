/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\antho                                            */
/*    Created:      Fri Jul 23 2021                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// LimitSwitchA         limit         B               
// Motor2               motor         2               
// Controller1          controller                    
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  Brain.Screen.render(true,false);

  //detects the state of the arduino switch and if it detects that the switch has been pressed, moves the door to the open position, if it detects that it's been unpressed the door closes
  while(true){
    if(LimitSwitchA.pressing()){
      Motor2.spinToPosition(3.2,turns, true);
    } else{
      Motor2.spinToPosition(0,turns, true);
    }
  }
}
