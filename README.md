RC_Switch
=========

RC_Switch with MSP430 LaunchPad


== Hardware Setup ==
* P1.0 LED output toggle
* P1.3 RC signal in

== RC Signal Note ==
* RC signal is 50Hz PWM
* 1 signal set is 20ms
* normal status is signal high time 1.5ms
* low status is signal high time 1.0ms
* high status is signal high time 2.0ms

== Program Note ==
* Normal Status is LPM4 (Low Power Mode 4)
* Interrupt trigger : RC signal input is LOW -> HIGH
* Read RC input port 20times between 100us (total 2000us = 2ms)
* Check how many High time.
* If High Status Read Result is more than 15 times, it's HIGH Status.
* If not, make LED Low Status

== More To Do ==
* Relay Control
* Tact Switch Mode
* Selectable Mode Change
