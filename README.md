# Semaphore-CMRI-JMRI
CMRI Three and Two position semaphore signal controler for use with JMRI.
Version 1.0 - Made 7-2022 by Anthony Kochevar 
with the help of a few friends who provided insight into CMRI detection, movement 
and the bounce function.
This sketch can be freely distributed and modified provided there is no cost.
Can be used with my 3D printable N scale semaphore signals found on
Thingiverse.

PLEASE READ ALL COMMENTS IN THE SKETCH THOUROGHLY.

JMRI INFO:  To correctly use three postion semaphore servos with JMRI 
each servo gets three CMRI node addresses.  In the example below we have
8 Three postion servos and 8 two position servos-LEDs.
Since the CMRI address below is 3 the starting CMRI address is 3001 for
the Port 0 (PCA9685 boards start at 0) servo is the first postion, 3002 for 
the middle postion and 3003 for the third postion.  3004 to 3006 is for Port 1 servo.  
3007 to 3009 for Port 2 and so on through all the ports.

    On two position servos they are still assigned three CMRI addresses but only the first one is used.
This was done so that adding or removing different types of semaphores
won't cause you to have to recalulate and reenter all the two position servos in JMRI.
In JMRI you will setup each postion as a "light" first. Then create a internal turnout 
for each "light" then last a light controler linked to the internal turnout with 
"thrown +" being items in JMRI you can now create a three output signal head.

    Be aware of which positon is which and plan accordingly.  In my case with the way
I mounted my three postion semaphore servo in a way that had red as the first and lowest value
position (3001) yellow the middle position (3002) and green as the third and highest
value positon (3003).  I had to plan accordingly when setting things up in JMRI.
In JMRI when a CMRI position address (light) is ON (Thrown) and the servo moves to that position
and sets the other two CMRI postion addresses to OFF (closed).

    A two positon semaphore is much simpler.  It only needs one CMRI address as it can be 
on or off, closed or thrown.  On two position servos they still get assigned three CMRI 
addresses but only the first one is used.  This was done so that adding or removing different 
types of semaphores (three or two position) won't cause you to have to recalulate and reenter 
all the two position servos CMRI addresses in JMRI.

In the example below Port 8 has CMRI address of 3025 to 3027, but only 3025 is used.
Port 9 has addresses 3028 to 3030, but only 3028 is used.  The other addresses become 
active or inactive as you add or remove signals via N_TRISERVO and N_BISERVO below.

You'll setup the two postion semaphores as a "light" just like above and also define a 
internal turnout.  You'll then create a light controler linked to the turnout.
You can then create a double output signal head using the light controled with one position 
as closed and the other as thrown.

Two position CMRI addresses can be used for switch track turnout servos and LEDs in JMRI.

If you change the number of three and two position servos below you will need to calculate the CMRI 
addresses accordingly.
