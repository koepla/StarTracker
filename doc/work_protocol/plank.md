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

---

## :memo: platform-io project, refactor

```
Date:           14.09.2021
Time spent:     0.5h
```
Added the PlatformIO project to the src Folder (microcontroller-code). I also made minor refactorings to the StarTracker source code and added a little stopwatch utility-class.

---

## :memo: started celestial body computation code

```
Date:           08.10.2021
Time spent:     4h
```
Today I started to work on the source code for computing right ascension and declination from given keplarian elements. It still has a few bugs but I've tracked the problem down to computing the mean anomaly. 

---

## :memo: first working celestial body computation code

```
Date:           09.10.2021
Time spent:     6h
```
Today I sort of completed the celestialbody computation code, it works for solar system bodies. I still had a few issues with transforming heliocentric coordinates to geocentric coordinates. Celestial bodies can now be loaded from a .json file, I added the major planets of our solar system.

---

## :memo: update on celestial body computation

```
Date:           10.10.2021
Time spent:     0.5h
```
Made a minor change for computing the solution for kepler's equation. 

---

## :memo: code documentation

```
Date:           30.10.2021
Time spent:     2h
```
On this day, I documented all major functions of the StarTracker codebase. 

---

## :memo: code documentation

```
Date:           02.11.2021
Time spent:     2h
```
Today I removed all external dependencies from our repository. StarTracker now uses vcpkg packages. I added build instructions to the readme of our repository. 

---

## :memo: bug in greenwhich mean sidereal time

```
Date:           02.11.2021
Time spent:     2.5h
```
Today I discovered a bug in our computation of the greenwhich mean sidereal time. Due to the change to winter-time, the code doesn't work anymore. I added a temporary fix: We can pass in the local time to the function that computes the greenwhich mean sidereal time, because this will already give us the local mean sidereal time. But we have to ditch the addition of the observer's longitude, which is by any means not optimal.

---

## :memo: first real test of StarTracker

```
Date:           02.11.2021
Time spent:     4h
```
Today we went out onto the fields for testing our prototype project. We detected a few inconveniences in the way which StarTracker performs movements. The photographing of celestial bodies didn't quite work due to our inability to operate the camera. We had an issue with focusing to infinity, and the images where blurred due to the manual shutter release. But we have to admit that we learned a lot for our project.

---

## :memo: functions for adding parts of time to DateTime, GMST fix

```
Date:           13.11.2021
Time spent:     2h
```
Today I implemented functions for adding parts of time (seconds, minutes, hours...) to our StarTracker::DateTime class. I used the TDD strategy and it worked out pretty well. These additions enabled me to implement a function which computes the difference to utc time, and then adds or subtracts the difference to a given time. This means that we can compute the relative UTC-time for any other time. We need this exact functionality for our greenwhich mean sidereal time issue, which I then went on to fix. 

---

## :memo: added support for fixed objects to celestial body class

```
Date:           13.11.2021
Time spent:     3h
```
Today I renamed the CelestialBody class to SolarSystemBody, in order to use the CelestialBody class as a base class, which directly reads right ascension and declination from the json file. We need this functionality for fixed bodies (Galaxies, Stars). I added some major galaxies and nebulas to the .json file. 

---

## :memo: worked on graphical user interface

```
Date:           14.11.2021 - 02.02.2022
Time spent:     30h
```
In this timespan, I started to implement the boilerplate code for the graphical user interface library of our choice: ImGui. The base structure for our GUI Application is the following: 
Everything is contained by a derived Class of StarTracker::Core::Application. This class houses the heavy lifting for the windowing as well as a list of views which are then drawn every frame. The font of our choice is San Francisco. 
In order to test our new GUI-System, I wrote a basic program which calculates the position of every celestial object in CelestialBodies.json. One can then select a body, which opens a popup to track the body. 
The tracking functionality is also new. One can specify the preferred CelestialBody, one's position, and the duration for the tracking process. The tracking function then starts a new thread, which repeatedly sends tracking commands to the microcontroller.

---

## :memo: refactor, tracker functionality update

```
Date:           05.02.2022
Time spent:     1h
```
Today I spent some time refactoring some code in the StarAPI. I decided that the Type 'CB' is not really fitting for FixedBody, so I changed that. I also made sure that we are consistent with initializers.
In order to check if the tracker is still connected, it is not enough to check the isOpen flag in the SerialPort class. I found a solution where we have to check if the handle is still valid, and then we can try to clear any communication errors by calling ClearCommError. This method fails if the SerialPort is not connected anymore.

---

## :memo: refactor, input class

```
Date:           06.02.2022
Time spent:     2h
```
Today I spent time refactoring parts of the CelestialBody class in the StarAPI. I detected inconsistencies in the way we handle exceptions, so I fixed that. In addition to that, I updated some methods in the SerialPort class, again related to Exceptions. Also there were some errors in the doc-strings. I added an Input class, where one can check if a specific key or mouse-button is pressed. One can request the mouse-position too. 

---

## :memo: ui-refactor, multi-threading, starapi-refactor, event-system

```
Date:           19.02.2022
Time spent:     5h
```
Today I started with refactoring some of the user-interface: The position of the user gets displayed in the ui. Then I went on and made sure, that only one tracking-proccess is allowed at a time. In order to realize that in code, I had to research a few things multi-threading related. I figured that an atomic-bool member is enough for this job. After I went through the code of the StarAPI a bit, I noticed that we were treating coordinate-structs as classes, so I changed them, because structs should obviously only carry data. This required me to perform some refactoring in the Transform class. After all this was done, I went through the pain of implementing a basic event-system. It is now possible for one to register an event-handler, which gets called whenever an event is fired. This is done by using a window-owned event-dispatcher. 

---

## :memo: opengl, utility

```
Date:           20.02.2022
Time spent:     6h
```
Today I spent quite some time implementing basic opengl functionality. It is now possible for one to create a vertex-buffer and index-buffer, tie them together in a vertex-array, write shaders, and specify the buffer-layout by using the buffer-layout class. I tried to write the code as expressive as possible, so that it is generally easier to deal with opengl. 

---

## :memo: refactor

```
Date:           22.02.2022
Time spent:     0.5h
```
Today I refactored some opengl code. 

---

## :memo: image-processing (waste of time)

```
Date:           23.02.2022
Time spent:     4h
```
On this day, I wasted a hell lot of time trying to implement boilerplate code for imageprocessing. I started by creating an image class, which could load images. Note that this class was really messed up, and ended up in the wrong directory. I had a very weird issue where renaming the directories in visual studio did not rename them in windows, but new classes ended up in the renamed directories, but the old ones did not. I deleted the new directory about 10 times, before I decided that I should call the day for today. 

---

## :memo: opengl textures

```
Date:           25.02.2022
Time spent:     2h
```
Today I implemented texture support for opengl. I tested the texture class by rendering the famous pillars of creation. 

---

## :memo: change to cmake

```
Date:           04.03.2022
Time spent:     3h
```
With my trauma of the 23.02.2022 in mind, I decided to dump visual studio solutions, and opened a new branch: the cmake-port. I removed the visual studio solutions, copied the file to new folders and added a basic cmakelists.txt. Note that I renamed the StarAPI to a more fitting StarEngine. After the port was done, I merged the branch with main.

---

## :memo: opengl-framebuffers, new root view

```
Date:           05.03.2022
Time spent:     3h
```
On this day, I implemented framebuffers for opengl, and I added a new root view, the StarTrackerView, which houses all other views and registers them to the viewList. Note that framebuffers do not work properly, but I think I've isolated the issue to the resize function.

---

## :memo: opengl-framebuffers fix, more mouse-events

```
Date:           06.03.2022
Time spent:     2h
```
Today I fixed framebuffers, the issue was that the framebuffer has to be recreated whenever it is resized. In addition to that I implemented more specific mouse-events in our event-system.

---

## :memo: basic opengl-camera

```
Date:           11.03.2022
Time spent:     2h
```
On this day I implemented a simple OpenGL camera, which enables one to fly around in the current scene. The camera registers an event-handler, which is only needed for zooming. Any other input gets handled by the Input class.

---

## :memo: image-processing, asset-database

```
Date:           12.03.2022
Time spent:     4h
```
Today I added an image-processing class, which can stack a list of textures on the gpu. It does that by binding the textures to the 32 available texture-units on the gpu, and then stacks them by averageing their color-values in the fragment-shader. I figured that it would be very handy to have an asset-database, which caches the loaded shaders and textures on the gpu.  

---

## :memo: image-processing-view, pain

```
Date:           14.03.2022
Time spent:     5h
```
On this day I nearly got a seizure. It started pretty good: I added a renderer class, which takes a vertexarray, shader, and primitive-enum and then draws them indexed. I then decided that I should move the image-processing stuff to a seperate view, so I went on and created the ImageProcessingView class. Here is were the pain started. Some textures caused a weird crash, whenever the function glTexImage2D was called. I went through 2 hours of debugging, and I do still not know what causes this crash, but I figured I should not dump the code. So I commited a crashing version of the ImageProcessingView class.

---

## :memo: texture-issue

```
Date:           16.03.2022
Time spent:     1h
```
Today I implemented a 'fix', where .png images with 3 channels get treated as GL_RED formatted. This fixes the crash on some images.

---

## :memo: texture-issue

```
Date:           17.03.2022
Time spent:     2h
```
On this day I fixed the texture-issue by telling stb-image that every texture should have 4 channels, so that we can treat every image as GL_RGBA formatted. 

---

## :memo: kernelled framebuffers

```
Date:           18.03.2022
Time spent:     3h
```
Today I implemented a new image-processing function, which enables one to apply a kernel to a source-framebuffer, which gets then rendered into a target-framebuffer. It did not work in the beginning, because I thought that I could handle it with only one framebuffer. Technically this could work, by having a copy-framebuffer in the background, where we copy the color attachment of the target-framebuffer to, and then render the kernelld copy-framebuffer into the target-framebuffer. But I think we will stick with the target and source version.

---

## :memo: major ui changes, image-processing refactor, tracker refactor

```
Date:           20.03.2022
Time spent:     6h
```
On this day I basically redid the whole ui, by using the ImGui window-drawlist, which enables us to draw beautiful ui-elements. It was a bit of a pain, but now the ui looks really nice. The trackable bodies are now bigger cards, which have an (for now hardcoded) texture attached to them. The name, designation and position preview is also on the card. In addition to that I decided that stacking only one texture is allowed now, alltough it doesn't make much sense. I also figured that the tracking duration should really be a double in the Tracker class.

---

## :memo: ui change

```
Date:           25.03.2022
Time spent:     2h
```
Today I redid some of the ui, I added a better looking info card for the TrackableBodyView class. 

---

## :memo: dark-mode, setup script

```
Date:           27.03.2022
Time spent:     2h
```
On this day I added a basic dark-mode, which I copied from https://github.com/TheCherno/Hazel. After dealing with some vcpkg related issues, I came to the conclusion that it would be smart to have a local vcpkg copy, where all of the dependencies get installed to, so a potential user which wants to build the project from source doesn't have to change the cmake toolchain file in their cmakelists.txt. 

---

## :memo: custom allocator, sanity checks in starengine

```
Date:           30.03.2022
Time spent:     2h
```
Today I added a custom Allocator class to track memory-leaks. In addition to that I added better sanity-checks in the CelestialBody class in the StarEngine project. 

---

## :memo: model-loading

```
Date:           01.04.2022
Time spent:     3h
```
On this day I implemented model-loading with the tinyobjloader library.

---

## :memo: model-loading, doc-strings

```
Date:           02.04.2022
Time spent:     4h
```
On this day I updated the model-viewer ui, fixed some issues in the model-loading and added doc-strings in every file in the StarTracker::Core and StarTracker::Util namespace. Note that I have not documented the OpenGL and Events files. 

---

## :memo: refactor, lighting, texture-handles

```
Date:           03.04.2022
Time spent:     6h
```
Today I spent maybe 1.5 hours reformatting our entire codebase, to use tabs instead of whitespaces for indenting. After that I figured that models do not really need a color vertex-attribute, so I loaded the normals instead, which enabled me to implement basic lighting. The rendered models look so much better know. After procastinating implementing textureHandles for CelestialBodies for ages, I decided to finally implement the textureHandles. I went to google and downloaded images for every CelestialBody that we currently have in our CelestialBodies.json file. I resized them al to 512x512 or 511x511 pixels, and exported them as .png images to our assets folder. Alltough it took nearly 2.5 hours, it was really worth it because the result is stunning to the eye. 