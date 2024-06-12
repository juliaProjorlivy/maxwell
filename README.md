# Maxwell Distribution Analysis
This project was made to study particle collisions and their Maxwell Distribution.
Starting to move at the same speed in a random directions, the particles
should come to a Maxwell velocity Distribution after some time. 
So the aim is to calculate that time, considering starting conditions (velocity, 
number of particles, Distribution density).
![](output.gif)

## Theory

Here some theoretical information:
![Maxwell Distribution](image/maxwell1.jpg)
![Maxwell Distribution](image/maxwell2.jpg)
![Balls collision](image/maxwell3.jpg)

## Stop program condition 
Apparently particles come to Maxwell Distribution somehow quickly.
So we can set fault and continue measuring time till calculated fault is less than the set one.

## Installation
#### Requirements
Some libraries: [mathGL](https://mathgl.sourceforge.net/doc_en/Main.html), [raylib](https://www.raylib.com/index.html), my own error managing lib that is already
in code as a submodule.
````bash
make
./maxwell
