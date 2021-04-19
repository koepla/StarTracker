## :memo: git, concept, project structure

```
Date:           16.04.2020		
Time spent:     2h 
```

Today we worked on creating a first draft of our concept and got a ruff idea on how our microcontroller protocol might work. We also created a GitHub organization and this repository.

---

## :memo: Fusion 360

```
Date:           17.04.2020		
Time spent:     3h 
```

Today I tested our first prototype and found that when a heavy camera is mounted there are vibrations when moving. I narrowed it down to the connection between the camera bracket and the stepper motors. Then I improved the camera bracket by adding two 1/4 inch screw holes to hold the stepper motor axises more firmly. That will hopefully eliminate vibrations while turning. 

---

## :memo: troubleshooting stepper drivers and testing new camera bracket design

```
Date:           18.04.2020		
Time spent:     6h 
```

Today I spent my time troubleshooting the stepper motors/drivers and I also discovered a week link in the camera bracket design.

While testing 3 DRV8825 stepper drivers broke to unknown reasons and generally their reliability wasn't great so we decided to switch to TMC2209 drivers because it seems like they are more robust and they also have a bigger feature set.

I also discovered a weak link in the camera bracket design. Because 3D-Printing is obviously layer based, the print splits between its layers when putting torque on the screw. We plan to resolve this by adding an aluminum extrusion around the 3D-Print where the screw is located so it can't split anymore.
Also I need to make slight modifications to the design do make the screws fit better.

---

## :memo: researching tmc2209 drivers

```
Date:           18.04.2020		
Time spent:     3h 
```

Today I spent a few hours learning about the tmc2209 driver. We have now decided for a library we are going to use because implementing the advanced features of this driver in Arduino code ourselves is out of scope for now.

---

