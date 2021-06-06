# Grand Prix

## Architecture
This grand prix simulator works by creating a determined number of racers in a rectangular track in order to compete. Each one of them is a struct named card and is managed using a combination of channels and Go Routines in order for them to work. 

## Racing track
The racing track is simpli a rectangular matrix, where there are two types of values, the default value simulating an empty space, and the ID of a racer, which represents his placement on the road. This was the most simple way to represent a track, since we can verify is two cars are about to collide with each other and do something about it.

## Car struct
This is a struct where each of the racers keeps his personal statistics.

* **ID**: It's assigned to each racer during initialization, and represents them onn the racing track
* **PositionX**: Their place on the X axis of the racing track matrix
* **PositionX**: Their place on the Y axis of the racing track matrix
* **MaxSpeed**: The car maximum available speed, it won't ever be possible to surpass this.
* **Rank**: Refers to current ranking in the race.
* **CurrentLap**: Current lap counter. 
* **IsFinished**: Boolean to determine those racer who have finished.
* **CurrentSpeed**: Variable used for the next movement of the car.
* **CurrentTime**: The timer for the current racer.

## Race structure
The race will continue until all of the racers have finished all of their laps, even if one car is reamining, the process will continue.

## Moving the cars
The program will be iterating between all the cars currently on the race, if a car has already finished, it will be skiped because no more processes concern them.
At the start, they will be given a new currentSpeed, where if they already have maximum speed, they will get a new randomized one between a minimum one and a maximum one, and if they are not at max speed, they will get an increase.
Then, we will be cheching for collision, where if one car would get inside another, they will instead try to avoid it to the side.


