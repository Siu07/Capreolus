//28/01/14
//Version 0.0.5
/* ______     ___      .______   .______    _______   ______    __       __    __       _______.
  /      |   /   \     |   _  \  |   _  \  |   ____| /  __  \  |  |     |  |  |  |     /       |
 |  ,----'  /  ^  \    |  |_)  | |  |_)  | |  |__   |  |  |  | |  |     |  |  |  |    |   (----`
 |  |      /  /_\  \   |   ___/  |      /  |   __|  |  |  |  | |  |     |  |  |  |     \   \    
 |  `----./  _____  \  |  |      |  |\  \-.|  |____ |  `--'  | |  `----.|  `--'  | .----)   |   
  \______/__/     \__\ | _|      | _| `.__||_______| \______/  |_______| \______/  |_______/    
                                                                                                   

      _______  __  .__   __.  _______     _______   ______     ______    _______       _______.
     |   ____||  | |  \ |  | |   ____|   |   ____| /  __  \   /  __  \  |       \     /       |
     |  |__   |  | |   \|  | |  |__      |  |__   |  |  |  | |  |  |  | |  .--.  |   |   (----`
     |   __|  |  | |  . `  | |   __|     |   __|  |  |  |  | |  |  |  | |  |  |  |    \   \    
     |  |     |  | |  |\   | |  |____    |  |     |  `--'  | |  `--'  | |  '--'  |.----)   |   
     |__|     |__| |__| \__| |_______|   |__|      \______/   \______/  |_______/ |_______/    */
/*
todo:
 Done 100%: average last 5 seconds of reading to give consistant result
 make user input possible                                            Note: Buy a better rotary encoder, consider a keypad matrix
 auto turn off backlight                                             Note: add timeout, think about pwm to fade screen out.
 upgrade power supply                                                Note: 1A should be able to keep 12v steady. Need to get 2.1mm socket
 Done 75%: make routines more efficient                              Note: Using TimeAlarms to provide screen updates at regular intervals rather than slow down processing code.
 Done 50%: add real menu                                             Note: Experiment with M2tklib more. Probably require complete restructure of code.
 integrate data logging
 integrate phone support
 only map input reading for display, use unmapped for calculations
 Done 75%: resume on power loss.                                     Note: Rudimentory systems, auto resumes previous program and step on power. No run time continuation, recorded step started from begining.
 Done 50%: add first boot mode to populate EEPROM.
 Done 100%: test "doubleMap" function.
 Note: Add "first boot" flag (with version number) to repopulate all EEPROM data on update or first run. 
 Replace fixed number of steps with dynamic array                    Note: creation of new program shall give the following options, enter step value, next step, finish, back. user may keep pressing next step until they have enough steps.
 */
/*
 EEPROM Memory Allocation:
  heKp = 0
  heKi = 4
  heKd = 8
  coKp = 12
  coKi = 16
  coKd = 20
  huKp = 24
  huKi = 28
  huKd = 32
  deKp = 36
  deKi = 40
  deKd = 44
  //Superseeded - below memory locations not true in MenuSystem
 0-119 = programs    
 0-23 = program 1
 24-47 = program 2
 48-71 = program 3
 72-95 = program 4
 96-119 = program 5
 
 120 = storedPrg
 121 = storedStep
 */
/*My Menu:
      Text = Fixed Selectable Title
      # #  =Non-selectable Information displayed
      <>   = User Input (Changes information displayed)
      [ ]   =  Dynamic Menu
      
      Select Program
              [list of Programs]            Set number of programs? List by default as "1,2,3,4,5" etc.
                      Run Program
                              #Status#
                      Edit Program
                              #Letters#
                              <Select letter>
                              <Next letter>
                              <Previous letter>
                              Confirm
                              Back
                                      #Setpoints#
                                      <Raise Time>
                                      <Lower Time>
                                      <Raise Temperature>
                                      <Lower Temperature>
                                      <Raise RH>
                                      <Lower RH>
                                      next step            //Turtles all the way down
                                      Confirm
                                      Back
                      Delete Program
                              Delete Confirmation
                              Back
                      Back
              Back
      Settings
              Heating
                      P
                              #P#
                              <Raise Value>
                              <Lower Value>
                              Back
                      I
                              #I#
                              <Raise Value>
                              <Lower Value>
                              Back
                      D
                              #D#
                              <Raise Value>
                              <Lower Value>
                              Back
                      Back
              Cooling
                      P
                              #P#
                              <Raise Value>
                              <Lower Value>
                              Back
                      I
                              #I#
                              <Raise Value>
                              <Lower Value>
                              Back
                      D
                              #D#
                              <Raise Value>
                              <Lower Value>
                              Back
                      Back
              Humidifying
                      P
                              #P#
                              <Raise Value>
                              <Lower Value>
                              Back
                      I
                              #I#
                              <Raise Value>
                              <Lower Value>
                              Back
                      D
                              #D#
                              <Raise Value>
                              <Lower Value>
                              Back
                      Back
              Dehumidifying
                      P
                              #P#
                              <Raise Value>
                              <Lower Value>
                              Back
                      I
                              #I#
                              <Raise Value>
                              <Lower Value>
                              Back
                      D
                              #D#
                              <Raise Value>
                              <Lower Value>
                              Back
                      Back
              Back
      Status
              #Status#
              Back
*/
