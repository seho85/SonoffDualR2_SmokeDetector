Moonraker Integration
=====================
The Sonoff Module is controlled by [Moonraker](https://github.com/Arksine/moonraker) which provides the Web Interface for controlling Klipper powered 3D Printers.

The integration is pretty straight forward an should also be usable by other devices that can be controlled over MQTT.

See [Moonraker Documentation - MQTT Device Configuration](https://moonraker.readthedocs.io/en/latest/configuration/#mqtt-device-configuration) and [Moonraker - [power]](https://moonraker.readthedocs.io/en/latest/configuration/#power) for configuration details.

The MQTT broker used for testing doesn't need any credentials for the connecting clients. 

**I would not recommend to run this in production state in this configuration.**

<pre>
[mqtt]
address: 10.0.1.98
port: 1883

[power relay1]
type: mqtt
command_topic: /3DPrinter/Relay1/Cmd
command_payload:
  {command}
retain_command_state: True
state_topic: /3DPrinter/Relay1/State

[power relay2]
type: mqtt
command_topic: /3DPrinter/Relay2/Cmd
command_payload:
  {command}
retain_command_state: True
state_topic: /3DPrinter/Relay2/State
</pre>

[mqtt]
------
Defines the MQTT broker settings.

[power]
-------
The *[power]* sections define a power switch that can be controlled per Web-UI.

* *type: mqtt*
    * The power is controlled over mqtt
* *command_topic*
    * The topic to which a command is send for toggling the power
* *command_payload*
    * The payload that is send to the *comand_topic*
        * using *{command}* will send:
            * *on* for switching on
            * *off* for switching off
* *retain_command_state*
    * defines that the retain bit is set in the MQTT message
* *state_topic*
    * the *state_topic* defines the topic that can be used to retrieve the actual state of the power switch.