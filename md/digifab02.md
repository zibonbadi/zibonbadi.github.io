# 2D and 3D CAD basics

This document will outline some basic projects which have been created using Computer Aided Design (CAD) software. As an example using 2D CAD methodologies it will document the creation of a template sketch for a three-dimensional object constructed out of simple two-dimensional plates using the LibreCAD software.

For three-dimensional CAD work, you will read about a sketch-based creation of a simple two-part lunchbox.

All sources will once again be available on the repository.

## 2D CAD with LibreCAD

*Coming soon*

## 3D CAD with Autodesk Fusion 360
   
As excercise I decided to create a two-part lunchbox in an industrial style of a size of 20cm x 20cm x 7cm (the final model will be 6cm in height as I intend the design to be securable using feet screws of an estimated height of 1cm). 
Inspiration for this design came from the [Commodore Amiga 500](https://en.wikipedia.org/wiki/Amiga_500) and [Atari 1040ST](https://en.wikipedia.org/wiki/Atari_ST) home computers.

First I started a sketch by laying out a base plane of 20cm by 20cm size, followed by two more squares with base size of 19cm x 19cm and 18cm x 18cm respectively. For alignment I used simple grid snapping. Afterwards I drew a 39° angle left of the 20cm square using the line and measurement tools which I applied a rectangular pattern to so that it covers the entire square in an evenl spread pattern.

![Lunchbox sketch](../img/lesson2/3dsketch.png)

After trimming the excess lines off for cleanliness I copied the base square over to the right and adjusted the right patterns 19cm square to a size of 18.9cm, such that the two halves will fit onto each other without issue.

After creating a secondary pattern for the front of the top half (using another rectangular pattern of horizontal lines) it was finally time to make my 2D sketch into a 3D model: First I extruded the entire sketch into a solid 20cm x 20cm x 5.5cm box.

Afterwards I created the groove patterns: Additively for the top by extruding select shapes up 6cm in connect/merge mode, then subtractively on the sides by extruding select shapes on the outside up 6cm in cut mode.

For making my lunchbox actually able to contain things, I needed to make it hollow inside as well as plan out a design to fit both sides of the box together: First I extruded the 18cm x 18cm square up 5cm in cut mode to hollow out the containment area. Next and finally for the top half, I extruded (cut mode) the are between the 18cm an 19cm squares to act as the inside walls of the lunchbox; 2cm up for the sides and 1cm up for the corners.

Now to the bottom side: Much like the top side I first created a -4.9cm extrusion as a base box to work with. Much like the 19cm inside square, I cut out 1mm as wiggle room for snugly fitting the halves onto each other. Afterwards I once again cut out the 18cm square as storage space followed by similarly extruding the walls in cut mode such that their height would be 0.9cm on the corners and 1.9cm on the sides.

As an afterthought I came up with the idea of letting the lunchbox stay on screw feet as an eye-pleasing way of securely locking it, so I modified the bottom halfs sketch by adding a nubbed screwhole of 4mm nub circumference (thankfully both sketches were exactly 30cm apart so I could copy the holes over to the other one precisely). After extruding the holes into each part appropriately Fusion 360 automatically settled for a 6H M2x0.4 right-turning winding for the top half. The bottom half did not receive a winding as it was not necessary and would've resulted in unnecessary screwing effort on behalf of the potential user.

![Screwhole of the top half](../img/lesson2/screwhole.png)

Below you can see a screenshot for the final result:

![Final lunchbox model](../img/lesson2/lunchbox.png)



