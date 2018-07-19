# 7/12
Q how does it sprite creation work?
A For example, when we load the ship instance in the LoadData function in the Game class we also create a sprite component. Creating a sprite component using a dependncy injection with a reference of the owning Actor object. The base component class attaches adds itself to the list of components in the actor's list of components.