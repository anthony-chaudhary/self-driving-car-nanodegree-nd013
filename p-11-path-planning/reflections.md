
## Reflections on path planning project



## Model overview


The control flow is generally:

* Input data from Simulator
* Update sensor fusion
* Behavior planning in frenet space
* Update our car state
* Store previous path as x y values in car space
* Generate jerk minmal trajectory coeffecients in frenet space
* Build coeffecients over time in frenet space
* Convert to xy car space
* Merge previous path and new path in car space
* Build spline
* Output global xy of spline to Simulator


## General reasoning

I felt it was important to do the planning in Frenet space. Frenet space is a mathematical transformation where we say "D" is the center of the road, and "S" is how far down the road we are. Our behavior planner and trajectory generator both work well in Frenet space.

The project is split into a variety of modules for a great degree of flexability. For example, we store all sensor fusion data at the start, and that data can then be accessed by a vareity of functions as the program progressses. There are a variety fo checks in place to prevent bad input from the simulator effecting output. For example, in the simulator the “speed” is unreliable, so instead we calculate it based off rate of change of position.

## Issues

The project was very challeneging. From a scope perspecitive and from bugs with the Simulator. 
For example, in a real car, we may wish to take our jerk minimal coeffecients and pass it to a MPC controller. In this case however, to deal with the neeeds of the simulator, the Spline acts as a pseudo controller.

Latency and computation time is incrediblly challenging. On my computer, every additional 10 trajectories generated during sub-processes cost about ~100ms of time. The simulator gets upset if things are out by 2ms. 

Further the simulator works differently on Windows and Linux, and generally is inconsistant. For no clear reason it will refuse to start, randomly stop, and/or randomly declare the car is over the speed limit when it’s clearly not.


## Learnings

I feel I have a much deeper understanding of path planning, trajectory generation, and value of computation time. 


## Further improvements

* Change Simulator code to accept throttle / yaw as input, and connect this to an MPC or PID controller.

* Finish implementing Naive bayes prediction module for other cars future behavior

* Improve behavior planning to more strongly represent differnet car states and issues


