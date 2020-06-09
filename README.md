# Ventilation-project

Ventilation system that has automatic and manual mode and can be controlled from lcd UI with buttons.


## Requirement
- Hardware
  - [LPC1549 LPCXpresso Board](https://www.nxp.com/products/processors-and-microcontrollers/arm-microcontrollers/general-purpose-mcus/lpc1500-cortex-m3/lpcxpresso-board-for-lpc1549:OM13056)
  
- Sofware
  - [MCUXpresso IDE](https://www.nxp.com/design/software/development-software/mcuxpresso-software-and-tools/mcuxpresso-integrated-development-environment-ide:MCUXpresso-IDE) to build the source code
  
- Automatic mode
  - Set the pressure level in the ventilation duct (0 – 120 pa) in the UI
    - Pressure level is pressure difference between the room and the ventilation duct
  - Controller measures pressure level and keeps the level at the required setting by adjusting the fan speed
    - If required level can’t be reached within a reasonable time user is notified on the UI
    
- Manual mode
  - Set the speed of the fan in UI (0 – 100%)
  - Display current fan setting and pressure level in the UI 
