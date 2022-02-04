Power Mode Sample Code
======================

NOTE: If you use this sample application for your own purposes, follow
      the licensing agreement specified in `Software_Use_Agreement.rtf`
      in the home directory of the installed Montana Software
      Development Kit (SDK).

Overview
--------
This sample project demonstrates a simple application that:

 1. Starts in Run Mode.

 2. Enters a pre-defined Power Mode (Sleep Mode or Storage Mode).
 
 3. Once any wakeup source is triggered, wakes up and goes back to Run Mode.
  
 4. Go to Step 1.  
 
The source code exists in `app.c`.

Options available for power modes:  
  - Sleep and wakeup from Flash with reset: in `app.h`, define SLEEP\_MODE\_TEST using
    SLEEP\_MODE\_TEST\_NO\_RETENTION.
  - Sleep and wakeup with core retention: in `app.h`, define SLEEP\_MODE\_TEST using
    SLEEP\_MODE\_TEST\_CORE\_RETENTION.
  - Sleep and wakeup from deep sleep: in `app.h`, define SLEEP\_MODE\_TEST using
    DEEP\_SLEEP\_TEST..
 
Different wakeup .sources available for Sleep Mode: 
- 	RTC alarm event: to enable it, in `app.h`, set WAKEUP\_SRC\_RTC\_ALARM\_EN to 1;
    By default, RTC alarm event duration is 125 msec.
    (Clock source, event duration can be set in `wakeup_source_config.h`)
    
  - FIFO full event: to enable it, in `app.h`, set WAKEUP\_SRC\_FIFO\_EN to 1;
    By default, FIFO event duration is 1 s. 
    (Clock source, state durations, and FIFO size can be set in
    `wakeup_source_config.h`)
 
  - Baseband timer event: to enable it, in `app.h`, set WAKEUP\_SRC\_BB_EN to 1;
    By default, baseband event duration is 2s. 
    (Deep sleep time can be set in `wakeup_source_config.h`)
 
  - ADC threshold event: to enable it, in `app.h`, set WAKEUP\_SRC\_ADC_EN to 1;
    By default, ADC threshold event duration is 500 msec. 
    (Number of samples, and ADC threshold value can be set in
    `wakeup_source_config.h`)
 
  - NFC wakeup event: to enable it, in `app.h`, set WAKEUP\_SRC\_NFC_EN to 1;
    Wakeup is generated when the phone with NFC enabled is placed 
    near the antenna.
 
  - GPIO event: by default, it is enabled; wakeup is triggered when there is
    a rising edge applied to the GPIO1 pin. 

This sample has two additional build configurations: Debug_Low_Power and 
Release_Low_Power. These build configurations turn off unused DRAM blocks
and only retain power for two DRAM blocks. 
These special configuration are applied for two sleep mode options:
SLEEP\_MODE\_TEST\_MEMORY\_RETENTION and SLEEP\_MODE\_TEST\_CORE\_RETENTION. 
These build configurations use a modified linker script file (sections_low_power.ld)
where the size of DRAM is set to two DRAM blocks.

The default TRIM values for VDDC and VDDM has been set to 1.15V and 1.10V
respectively in order to support reliable operation during extended temperature.
This values can be further reduced depending on the operating temperature of
the device to reduce the overall power consumption.

Different wakeup sources available for Storage Mode: 
  - Sensor detected event: to enable it, in `app.h`, set WAKEUP\_SRC\_SENSOR\_DETECTION_EN to 1; 
    Wakeup is generated when the equivalent resistance between 
    the WE and RE terminals is reduced. 
  
  - NFC wakeup event: to enable it, in `app.h`, set WAKEUP\_SRC\_NFC to 1; 
    Wakeup is generated when the phone with NFC enabled is placed near the antenna.
  
  - GPIO event: by default, it is enabled; wakeup is triggered when there is
    a rising edge applied to the GPIO1 pin. 
    
Sensor Calibration Mode
  - by default sensor calibration mode is enabled when WAKEUP_SRC_ADC is configured
  - sensor calibration mode can be disabled by setting value of SENSOR_CALIB to 0 in `app.h`
  - when SENSOR_CALIB is set to 0 user needs to make sure that there is atleast 10M ohm resistor
    between WE and RE pad so as to mimic sensor is present between these two pad
      
Hardware Requirements
---------------------
This application can be executed on an Evaluation and Development Board.

Importing a Project
-------------------
To import the sample code into your IDE workspace, refer to the 
*Getting Started Guide* for your IDE for more information.

Verification
------------

This application uses the following GPIO pins:
* RECOVERY\_GPIO (GPIO0) - To activate the Recover Mode.
* GPIO1: Wakeup GPIO pin.
* SYSCLK\_OUT\_GPIO (GPIO2) - To output the system clock. 
* POWER\_MODE\_GPIO (GPIO7) - To indicate Power Mode.
* WAKEUP\_ACTIVITY\_FIFO\_FULL (GPIO5) - To indicate FIFO FULL wakeup event
* WAKEUP\_ACTIVITY\_GPIO (GPIO4) - To indicate GPIO1 wakeup event
* WAKEUP\_ACTIVITY\_RTC (GPIO6) - To indicate RTC wakeup event
* WAKEUP\_ACTIVITY\_BBTIMER (GPIO4) - To indicate BB Timer wakeup event
* WAKEUP\_ACTIVITY\_THRESHOLD (GPIO5) - To indicate Sensor Threshold wakeup event
* WAKEUP\_ACTIVITY\_SENSOR\_DET (GPIO4) - To indicate Sensor Detection wakeup event

Three different sleep options are available to be configured:
- DEEP\_SLEEP\_TEST (lowest possible power consumption with this mode)
- SLEEP\_MODE\_TEST\_NO\_RETENTION
- SLEEP\_MODE\_TEST\_CORE\_RETENTION

To verify that this application is functioning correctly, trace all signals
as follows:
- Run Mode:
    - SYSCLK\_OUT\_GPIO: system clock is 3 or 8 MHz 
    - POWER\_MODE\_GPIO: low     
    
- Power Mode: 
    - SYSCLK\_OUT\_GPIO: off for the whole sleep duration
    - POWER\_MODE\_GPIO: high

The system keeps cycling through Run and Power modes. The timing of Run-Power
cycles depends on the configured duration of the possible wakeup source events. 
Note that after wakeup by NFC the system will stay in Run mode for the defined amount of 
seconds in APP\_DELAY\_S before going back to sleep. When there is a rising edge applied 
to the WAKEUP pad or the selected GPIO pin, the system wakes up and goes back to Power Mode.

Notes
-----
Sometimes the firmware cannot be successfully re-flashed, due to the
application going into Power Mode or resetting continuously (either by design 
or due to programming error). To circumvent this scenario, a software recovery
mode using RECOVERY_GPIO can be implemented with the following steps:

1.  Connect RECOVERY_GPIO to ground.
2.  Press the RESET button (this restarts the application, which pauses at the
    start of its initialization routine).
3.  Re-flash the board. After successful re-flashing, disconnect RECOVERY_GPIO from
    ground, and press the RESET button so that the application can work
    properly.

***
Copyright (c) 2021 Semiconductor Components Industries, LLC
(d/b/a onsemi).