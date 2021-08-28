## :memo: concept, project structure, git

```
Date:           16.04.2021		
Time spent:     2h 
```
Today we worked on creating the concept for our thesis, collected some ideas for the microcontroller-protocol and set up the git structure.

---

## :memo: microcontroller-protocol

```
Date:           17.04.2021		
Time spent:     3h 
```
Today I worked on implementing the microcontroller-protocol. I felt a bit stuck due to the non continuos memory of std::vector, but in the end I could solve it by using a stack allocated buffer, which recieves its size by a template specifier

---

## :memo: microcontroller serial communication

```
Date:           18.04.2021		
Time spent:     6h 
```
Today I implemented a class for serial communication with the micro controller. I faced a few problems with synchronization, timeouts etc.. Altough I wasted a couple of hours, in the end I was successful and the class works just fine.

---

## :memo: microcontroller serial communication

```
Date:           16.04.2021		
Time spent:     4h 
```
Today I added a platform io project, and implemented the stt protocol for the arduino. I set up a small test environment, where the eight recieved floating point numbers are printed on a lcd  display. I detected a small error, because I had to specify the size for the header's stt_flag. It is a two byte integer by default, but we want 4 bytes because we want to avoid alignment issues in the memory. The communication works very well, altough i realised that the arduino's input buffer for serial communication is only 64 bytes. I've left a comment in the stt_protocol.h file in the master program, where I describe the issue in greater detail.

---

## :memo: serial class, event system, app class

```
Date:           23.04.2021		
Time spent:     3h 
```
Today I spent some time on refactoring the code a bit. I changed the is_open function to check  now for an invalid handle. I also added in a very basic event system with callbacks. I spent the majority of the time on implementing the app class.

---

## :memo: planetary api

```
Date:           25.04.2021		
Time spent:     4h 
```
Today I worked on implementing the planetary api.

---

## :memo: testing tmc2209 drivers

```
Date:           25.04.2021		
Time spent:     5h 
```
Today we tested our new tmc2209 drivers with some simple code. At first we had some schlagerls but in the end everything worked.

---

## :memo: algorithm for computing a planet's / star's position

```
Date:           02.05.2021		
Time spent:     10h
```
For the last two days, I worked on the algorithm for computing a planet's or a star's position in the sky. In the beginning I faced some problems with figuring out the julian day number, then I detected some errors in my math helper functions, and I got my position coordinates wrong. 
The time was really worth it because now, we can compute any star's or galaxy's or planet's position with only its right ascension and declination given. Of course we need the time for the computation and the observers position too. 

---

## :memo: microcontroller driver code, protocol

```
Date:           17.08.2021		
Time spent:     2h
```
Today I refactored the Protocol class on both microcontroller and client side. I also tried to write a driver for our TMC Stepper Drivers, which has some issues, but I think I isolated the problem to the Move function.

---

## :memo: microcontroller driver code, visual studio solution, http-requests, location-service...

```
Date:           18.08.2021 - 28.08.2021		
Time spent:     40h
```
In the specified timespan I fixed the driver code for the TMC Stepper Drivers, the Issue was that I sent 64 bytes of data, but the Serial Buffer of the Arduino can by default only hold 63 bytes. After the driver code was fixed I created the Visual Studio Solution for the real source code. I implemented the now called StarAPI (Ephemeris computation framework) as a static library which is referenced by the StarTracker Project. In order to fetch ephemeris data from Webservices, I implemented a class to send HttpRequests, I later used this functionality to implemented the GeoLocation class, which returns approximated Location data. 