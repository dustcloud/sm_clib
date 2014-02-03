/*
Copyright (c) 2014, Dust Networks.  All rights reserved.

Arduino sketch which connects to a SmartMesh IP manager, retrieves the list of
motes connected to it, and sets/clears the motes in a round-robin fashion.

Note: before you can run this sketch, you need import the sm_clib
library. To do so:
- double click on this file to open the Arduino IDE
- In Sketch > Import Library... > Add Library..., select navigate to the
  sm_clib folder and click open.

Note: before running this sketch:
- configure your SmartMesh IP motes to run in master mode
- Configure the same network ID on all your motes and manager.
- remove the battery from your SmartMesh IP manager, it will be powered by the
  Arduino Due.
- Connect your Arduino Due board to your SmartMesh IP manager as detailed in
  the documentation.
- make sure the power switch of the SmartMesh IP manager is in the ON position.
- plug the Arduino Due in your computer, using a USB cable connected to the
  "programming" USB port.
- Switch on all your motes.

To run this sketch, connect your Arduino Due board to your computer over the
programming USB port, and Select File > Upload. 
  
\license See attached DN_LICENSE.txt.
*/

#include <IpMgWrapper.h>
#include <dn_ipmg.h>

IpMgWrapper       ipmgwrapper;

//=========================== "main" ==========================================

void setup() {
   ipmgwrapper.setup();
}

void loop() {
   ipmgwrapper.loop();
}
