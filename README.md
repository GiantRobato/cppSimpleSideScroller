# CPP SideScroller
Simple side scroller made in C++ with visual studios using SDL and C++11.

![Screenshot](Asteroids/Assets/screenshot.png)

# Classes

* Actor - base class for all Actors
* Game - initializes renderer + Actors and runs main update loop
* Main - simple wrapper around the game loop
* Math - From Game Programming in C++ by Sanjay Madhav (see file for licensing)

# Licensing

- Background sprites were made by [Jacob Zinman-Jeanes](http://jeanes.co) licensed under [CC BY 3.0](http://creativecommons.org/licenses/by/3.0/)
- Math.h and derivative files by Sanjay Madhav (see LICENSE2.txt)

# 7/12
**Q:** how does it sprite creation work?

**A:** For example, when we load the ship instance in the LoadData function in the Game class we also create a sprite component. Creating a sprite component using a dependncy injection with a reference of the owning Actor object. The base component class attaches adds itself to the list of components in the actor's list of components.