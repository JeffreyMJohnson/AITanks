# AI Tanks
Tank demo for applying AI / path finding algorithms.
#### Programmer - Jeffrey M Johnson ####
### Path Finding Demo ###
* There are "bases" in each corner of the map colored red.  
* The map is also populated with unwalkable "walls" colored brown, and "resources" colored green randomly placed.  
* A Blue tank collects resources by selecting a path to the nearest resource from the base it is at (using A* method).  
* When at a resource it collects over time which is listed in UI as Tank.  
* The resource has a finite quantity of resources and it will run out if collected.  
* The color of the resource bleeds to white as resource is collected.  
* When the tank is full, it will choose a path (using Daijkstra method) to the nearest base to deposit it, noted in UI Total.

### Steering behaviour demo ###
**Tag**
* Two tanks, red and green are placed randomly on the map.  
* The red tank is the seeker using seek steering behaviour, and the green tank is the fleer using flee steering behaviour.  
* When the red tank catches the green tank, they swap sides and resume chase after a time.  
* If the fleeing tank is not "visible" of seeker, it wanders using wander steering behaviour.

**Wander**
* 10 tanks placed randomly on the map are wandering using wandering steering behaviour.

### To-Do ###
fix bug when resources are done.

