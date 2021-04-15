# StarTracker - Concept :telescope: 

For our diploma thesis, we chose a project that covers many aspects including the following:

 - Programming
 - Mathematics
 - Physics
 - Electronics
 - Engineering

## Motivation

Our goal is to provide an affordable easy to use product for astrophotography hobbyists that achieves stunning performance for a reasonable price.

To achieve this goal we are relying on a simple, but robust 3d-printed structure, a well thought through pcb, controlled by an efficient but extensive collection of C++ code.

## Implementation 

The C++ code is split up into a microcontroller-interface, a graphics framework and an ephemeris-computation framework.

For the microcontroller-interface, we developed a robust communication protocol for transferring data over a serial interface.

The graphics framework provides an abstraction layer for OpenGL and ImGui, which enables one to perform modifications to the graphical user interface of the program.

The ephemeris-computation framework uses a set of equations for computing the position of a given planet or star. 

### Microcontroller-Protocol

Protocol ideas:

```
HEADER:     
FLAG        MOVE     => instruction for moving over angles x in timespan dt
            STOP     => instruction to stop the current movement
            ORIGIN   => stop current movement and return to angle 0, 0 
            SLEEP    => send stepper-motor x to sleep
            WAKEUP   => wake up stepper-motor x
SIZE        Content size in bytes 

BODY:       (dependent on the flag)
CONTENT     MOVE:    => list of 4 byte float angles -> last 4 bytes are the timespan as 4 byte float
            STOP:    => empty
            ORIGIN:  => empty
            SLEEP:   => list of 1 byte integers for the stepper motors, can only be 0,1 or 2
            WAKEUP:  => list of 1 byte integers for stepper motors that need to be woken up, can only be 0,1 or 2
```