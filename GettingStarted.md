# Introduction #

Short introduction to Arduino, getting it working on your system and what can be done with it.

For a [documentary](http://arduinothedocumentary.org) on the history and current state of the Arduino, see the following film by [Rodrigo Calvo](http://rodrigocalvo.com/index2.html) and Raul Alejos:

[Vimeo](http://vimeo.com/18539129)

Or the [TED Talk by Massimo Banzi](http://www.ted.com/talks/massimo_banzi_how_arduino_is_open_sourcing_imagination.html)

# Details #

## Arduino in a nutshell ##

As defined by [arduino.cc](http://arduino.cc/): "Arduino is an open-source electronics prototyping platform based on flexible, easy-to-use hardware and software. It's intended for artists, designers, hobbyists, and anyone interested in creating interactive objects or environments."

It offers an inexpensive way to get into [physical computing](http://en.wikipedia.org/wiki/Physical_computing), in other words how to make different kinds of devices to understand each other. This can mean in practice controlling robots, reading temperature/environmental sensors, monitoring physiological signals such as heart rate or brain activity, etc.

Traditionally, the systems for this have been cumbersome to use and not that "wallet-friendly". In this project Arduino is more or less used as an alternative for more expensive data acquisition devices (DAQ), the cost of one [Arduino Uno](http://arduino.cc/en/Main/ArduinoBoardUno) (˜20 eur) is reasonable compared to ˜600 eur of [USB DAQ](http://sine.ni.com/nips/cds/view/p/lang/en/nid/203223) for example from [National Instruments](http://www.ni.com/). However, Arduino is not strictly speaking a DAQ card and not all DAQ solutions are as expensive as NI's and for example [LabJack](http://labjack.com/) and [Measurement Computing](http://www.mccdaq.com/) offer more affordable solutions (and for more solutions you can check for example the [list of supported hardware](http://www.mathworks.com/products/daq/supportedio.html) for [Matlab](http://www.mathworks.com/)'s [Data Acquisition Toolbox](http://www.mathworks.com/products/daq/))

In addition to the [open-source](http://en.wikipedia.org/wiki/Open-source_hardware)] official [Arduino boards](http://arduino.cc/en/Main/Hardware), there are various "[Arduino-compatible](http://en.wikipedia.org/wiki/Arduino-compatible_boards)" boards that uses the same program code and compatible hardware broadening the selection for the end-user.

For a video tutorial on Arduino, see the first part part of 14 video tutorials by [Jeremy Blum](.md):

<a href='http://www.youtube.com/watch?feature=player_embedded&v=fCxzA9_kg6s' target='_blank'><img src='http://img.youtube.com/vi/fCxzA9_kg6s/0.jpg' width='425' height=344 /></a>

## Where to get more information ##

The official home page of [arduino.cc](http://arduino.cc/) is a good place to start with its [Playground](http://arduino.cc/playground/) offering [manuals](http://arduino.cc/playground/Main/ManualsAndCurriculum), [tutorials](http://arduino.cc/playground/Main/TutorialList), instructions how to interface with [software](http://arduino.cc/playground/Main/InterfacingWithSoftware) and [hardware](http://arduino.cc/playground/Main/InterfacingWithHardware), etc.

Other good sites for tutorials and basic "getting started" information is provided in the following sites:
  * [Adafruit's tutorials](http://www.adafruit.com/tutorials)
  * Arduino projects at [Instructables.com](http://www.instructables.com/technology/arduino/)
  * [MAKE Magazine's](http://makezine.com/) section on [Arduino](http://makezine.com/arduino/)

## Where to buy? ##

See the distributor list at [arduino.cc](http://arduino.cc/en/Main/Buy) for your closest store.

## Example applications / projects ##

### Biomedical ###

  * Collection of projects with Arduino for biosignals: "[Medical and Health Related Projects with Arduino](http://medicarduino.net/)"
  * [Open Sensors](http://sourceforge.net/p/opensensors/home/Home/): "The purpose of this site is to centralize the development of open source physiological sensors"
  * [makezine.com](http://makezine.com/)'s [Vol. 26: Biosensing](http://makezine.com/26/primer/): Track your body's signals and brain waves and use them to control things. By Sean M. Montgomery and Ira M. Laefsky.

  * [EEG With an Arduino](http://sites.google.com/site/chipstein/home-page/eeg-with-an-arduino) - [chipstein](http://sites.google.com/site/chipstein/)
  * [Ambulatory blood pressure monitoring](http://code.google.com/p/wombat-abpm/) (wombat-abpm)
  * [Analyzing logic with Arduino](http://jdesbonnet.blogspot.com/2010/05/how-to-make-ambulatory-blood-pressure.html) of the low-cost (˜17eur) Health & Life Co HL168Y Blood Pressure Monitor
  * [Infrared heart monitor](http://www.wonderhowto.com/how-to-make-infrared-heart-sensor-286365/)
  * [Indoor air quality](http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1271410369)
  * [Body temperature measurement](http://forum.hackedgadgets.com/viewtopic.php?t=2631&start=0&postdays=0&postorder=asc&highlight=&sid=dd2aeb2637700d86145def76d6f0f1da)
  * [Adafruit tutorial on temperature sensors](http://www.ladyada.net/learn/sensors/tmp36.html)
  * [Portable iButton Data Logger Programmer with Arduino](http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1271992724)
  * [DIY Actigraphy with Arduino](http://www.madedition.com/2011/diy-actigraphy-with-arduino) (with light and temperature)
  * [Light meter (lux)](http://www.microcodes.info/light-meter-circuit-10450.html)
  * [Heart rate monitor interface](http://danjuliodesigns.com/sparkfun/hrmi.html) by [danjuliodesigns llc](http://danjuliodesigns.com) using [Processing](http://www.processing.org/)
  * [Galvanic Skin Response Sensor with Arduino on your iPhone](http://medicarduino.net/?p=182)
  * [DIY Muscle Sensor / EMG Circuit for a Microcontroller](http://www.instructables.com/id/Muscle-EMG-Sensor-for-a-Microcontroller/) at [Instructables](http://www.instructables.com/)
  * [Lolo's Alarm Clock](http://code.google.com/p/lolosalarmclock/): "A fancy alarm clock built with the Arduino platform and hardware. It keeps track of time and can measure temperature, humidity, dew point, light level, air quality, and sleep patterns. It's designed as a wooden box with touch sensing for control."
  * "Concept post" on "[MindFlex + Arduino + Android = Sleep Quality Monitor & Sleep Analysis tool](http://www.huanix.com/2011/01/29/mind-flex-arduino-android-sleep-quality-monitor-sleep-analysis-tool/)" on using Mattel's [MindFlex](http://mindflexgames.com/) to monitor sleep quality via electroencephalography (EEG) recording (polysomnography).