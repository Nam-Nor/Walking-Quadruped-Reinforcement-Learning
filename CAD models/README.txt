                   .:                     :,                                          
,:::::::: ::`      :::                   :::                                          
,:::::::: ::`      :::                   :::                                          
.,,:::,,, ::`.:,   ... .. .:,     .:. ..`... ..`   ..   .:,    .. ::  .::,     .:,`   
   ,::    :::::::  ::, :::::::  `:::::::.,:: :::  ::: .::::::  ::::: ::::::  .::::::  
   ,::    :::::::: ::, :::::::: ::::::::.,:: :::  ::: :::,:::, ::::: ::::::, :::::::: 
   ,::    :::  ::: ::, :::  :::`::.  :::.,::  ::,`::`:::   ::: :::  `::,`   :::   ::: 
   ,::    ::.  ::: ::, ::`  :::.::    ::.,::  :::::: ::::::::: ::`   :::::: ::::::::: 
   ,::    ::.  ::: ::, ::`  :::.::    ::.,::  .::::: ::::::::: ::`    ::::::::::::::: 
   ,::    ::.  ::: ::, ::`  ::: ::: `:::.,::   ::::  :::`  ,,, ::`  .::  :::.::.  ,,, 
   ,::    ::.  ::: ::, ::`  ::: ::::::::.,::   ::::   :::::::` ::`   ::::::: :::::::. 
   ,::    ::.  ::: ::, ::`  :::  :::::::`,::    ::.    :::::`  ::`   ::::::   :::::.  
                                ::,  ,::                               ``             
                                ::::::::                                              
                                 ::::::                                               
                                  `,,`


http://www.thingiverse.com/thing:38159
Mini Quadruped - optimized for 9G servos by TheCase is licensed under the Creative Commons - Attribution license.
http://creativecommons.org/licenses/by/3.0/

# Summary


droger's adaptation of andromeda's quadruped is awesome!  I was planning on making my own design months ago.  Life got in the way for a while, and BOOM, someone saved me a ton of time!  A heartfelt thank you for that :)  

Small problem though - the original design didn't fit the cheap 9G micro servos that I had on hand.  

So I spent a *smaller amount of time* optimizing the design to fit the servos I (and a lot of other people might) have in stock.  

I zipped up the OpenSCAD versions of the parts in case you need to tweak them to fit your servos or printer tolerances.  

NOTE:  what's shown in the picture are some mid-prototype parts.  The finals are actually nicer with rounded corners and whatnot.  Fancy!  

UPDATE UPDATE: I cleaned up the OpenSCAD files in the zip package.  I know some of you have servos with slightly different dimensions.  The new scads should be parametric so that you can tweak  a few dimensions to get parts that will fit your servos!  

UPDATE UPDATE UPDATE: the parametrics now support different servo horn dimensions!  

UPDATE UPDATE UPDATE UPDATE:  I've included a beta version of the design intended for those that have the Tower Pro version of the blue servo, based on communicated measurements.  No guarantees: I do not have one of these servos to test the fit.  I do have some on order, but it could be a couple weeks before they arrive.  The beta STLs are available in the zipped archive named case_qp_tower_stls-beta.zip .  

# Instructions

4 pcs leg
2 pcs hip
1 pcs backbone
8 pcs shoulder
8 pcs servo button

The servo buttons go in the hole in the shoulder that is opposite of the servo horn.  They act as a bearing for the back of the servo as it moves.  In a pinch (if your printer has issues printing these tiny bastards), you might be able to get away with some clipped up bits of the unused horns included with your servos.

I designed the junction of two shoulder pieces to be held by two 3mm x 6mm screw/nut each diagonally offsetting from different directions. 

I derived the code from lumi's wonderful "chopstick jr" on letmakerobots.com http://letsmakerobots.com/node/29708

UPDATE:  I've messed around with some of the code - at least its a start.  I have only worked on the forward movements.  Still nothing solid.  I attached it if others would like to help improve it.  Note that in the top section there are 'home' values for each servo.  While I did center my servo horns on the mechanical 'center' of the servo, when I set the value to 90 degrees in the code, the servos were not at center position.  I had to adjust these home values to center them.  YMMV

For those wondering about the electronics and the wiring - I've just added an archive with an Eagle schematic and board file.  Note that I didn't work out the traces on the board.  I wired mine by hand on a protoboard, but it'll give you an idea if you're confused about how to connect things.

I soldered female pin headers to accept the Arduino Nano on a small perf board.  I soldered in three rows of pin headers across the back of the D5-D12 pins (which will talk to the servos).  The first row is connected to the Ardiuno pins, the +5v in the next position and GND on the last.  

I'm using four (4) 1.2v rechargeable AA batteries in a 2x2 snap-in holder for power.  Wired directly to the 5v rail of the Arduino.  I had previously tried a 9v battery tied to the VIN line, but the Arduino's voltage regulator couldn't handle the servos and kept rebooting the board every time it made its first movement.

The eyes consist of a Ultrasonic Module HC-SR04 Distance Sensor link: http://goo.gl/lUK4o .  TRIG is wired to D13 and ECHO is wired to D3.   You can choose whichever pins you like, just make sure to change the code to reflect this.