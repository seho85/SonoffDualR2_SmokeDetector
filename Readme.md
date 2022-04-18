>  **DISCLAIMER:**
>   This project project is made by an hobbyist.
>   Everything described here is without guarantee, so if you perform something described here, **YOU DO IT ON YOUR OWN RISK**
>

Sonoff Dual R2 with Smoke Detection Support
===========================================

**WARNING**
-----------

**The Sonoff Module is connected to 230V, contact to the wrong components could cause serious injuries.**

**So I decided to keep out the 230V connection part of the whole description.**

**Please get some electrician to install the high voltage part!**

**Due the caveats I discovered on the first tests I would consider this whole project not production ready - before i'll start using this for my 3D printer the MQTT part will be rewritten, that now reset should be triggered when the MQTT Broker becomes unreachable, or the WiFi could not be accessed anymore**

Goals
-----
I tried to find a cheap solution that turns of the power of my 3D Printer when smoke was detected. It would be possible to buy one of the "smart smoke detector" but they weren't very cheap to get. 

And I also prefer to connected the smoke warning by a wire to the Sonoff module. 

In the actual way: **It's NOT wire break safe**

The power of my 3D printer should be controlled by an [Sonoff Dual R2](./doc/SonoffDualR2.md) module, which is basically an ESP8285, with two Relais and few components to make everything work.

The Sonoff is controlled per MQTT

Here you can find everything I discovered to make this working.

Caveats
-------
The actual implemenation has some now caveats as they were:

* Connection from smokesensor to Sonoff Module has no "Wire Break Detection"
* **Slow Reaction time on smokesensor alarm.** When the smoke sensor issues the alarm, the library used for mqtt communication delays the disabling the relays for up to ~7 seconds.
    * I suspect this behavior from the MQTT library that is currently used. But I found no easy way to decrease that delay.
* Not long term testing approved
* **When no MQTT Broker is available the Sonoff will perform a reset (resulting in switched off Relays and cutted powersupply of the 3D printer.)**
    * This definitly caused by the MQTT library used. 


If someone has solved one of this problems, and is willed to share the solution, please contact me.


Components used
---------------

* [Grundig Smoke Detector](./doc/GrundigSmokeDetector.md) (~5€)
* [Sonoff Dual R2](./doc/SonoffDualR2.md) (~15€)

Sofware used
------------

* ESP Arduino framework
    * [Arduino Code](./doc/ArduinoProgram.md)
    * [MQTT Topics](./doc/MQTT_Topics.md)
* mosquitto MQTT broker


Wiring
------
See [Wiring](./doc/Wiring.md)

Integration in Moonraker
------------------------
See [Moonraker Integration](./doc/MoonrakerIntegration.md)