## :memo: concept, project structure, git

```
Date:           16.04.2020		
Time spent:     2h 
```
Today we worked on creating the concept for our thesis, collected some ideas for the microcontroller-protocol and set up the git structure.

---

## :memo: microcontroller-protocol

```
Date:           17.04.2020		
Time spent:     3h 
```
Today I worked on implementing the microcontroller-protocol. I felt a bit stuck due to the non continuos memory of std::vector, but in the end I could solve it by using a stack allocated buffer, which recieves its size by a template specifier

---

## :memo: microcontroller serial communication

```
Date:           18.04.2020		
Time spent:     6h 
```
Today I implemented a class for serial communication with the micro controller. I faced a few problems with synchronization, timeouts etc.. Altough I wasted a couple of hours, in the end I was successful and the class works just fine.

---

## :memo: microcontroller serial communication

```
Date:           16.04.2020		
Time spent:     4h 
```
Today I added a platform io project, and implemented the stt protocol for the arduino. I set up a small test environment, where the eight recieved floating point numbers are printed on a lcd  display. I detected a small error, because I had to specify the size for the header's stt_flag. It is a two byte integer by default, but we want 4 bytes because we want to avoid alignment issues in the memory. The communication works very well, altough i realised that the arduino's input buffer for serial communication is only 64 bytes. I've left a comment in the stt_protocol.h file in the master program, where I describe the issue in greater detail.

---

## :memo: serial class, event system, app class

```
Date:           23.04.2020		
Time spent:     3h 
```
Today I spent some time on refactoring the code a bit. I changed the is_open function to check  now for an invalid handle. I also added in a very basic event system with callbacks. I spent the majority of the time on implementing the app class.

---

## :memo: planetary api

```
Date:           25.04.2020		
Time spent:     4h 
```
Today I worked on implementing the planetary api.

---

## :memo: testing tmc2209 drivers

```
Date:           25.04.2020		
Time spent:     5h 
```
Today we tested our new tmc2209 drivers with some simple code. At first we had some schlagerls but in the end everything worked.

---