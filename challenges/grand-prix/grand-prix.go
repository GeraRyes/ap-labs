package main

import (
	"flag"
	"fmt"
	"math/rand"
	"os"
	"os/exec"
	"sort"
	"sync"
	"time"
)

var laps int

var players []Car

var racingTrack [5][90]int

var wg sync.WaitGroup

var startTime time.Time

var mu sync.RWMutex

type Car struct {
	id           int
	positionX    int
	positionY    int
	maxSpeed     int
	rank         int
	currentLap   int
	timer        string
	isFinished   bool
	currentSpeed int
	currentTime  float64
}

func createRacers(numberRacers int) {
	var row int = -1
	var column int = 0
	minSpeed := 6
	maxSpeed := 12

	for i := 0; i < numberRacers; i++ {
		if (i % len(racingTrack)) == 0 {
			row++
			column = 0
		}

		//Initialize racer
		var racer Car
		racer.id = i + 1
		racer.currentLap = 0

		rand.Seed(time.Now().UnixNano())
		racer.maxSpeed = rand.Intn(maxSpeed-minSpeed) + minSpeed
		racer.rank = 1
		racer.positionX = row
		racer.positionY = column
		racer.isFinished = false
		racer.timer = ""
		racer.currentSpeed = 0

		racingTrack[column][row] = racer.id
		players = append(players, racer)
		column++
	}
}

func play(ranking chan Car) {
	startTime = time.Now()

	//If not all of the players have finished
	for !allPlayersFinished(ranking) {

		//Checks for the movements of all players and makes them wait
		for i := 0; i < len(players); i++ {
			wg.Add(1)
			go moveCars(&players[i], ranking)
		}
		time.Sleep(time.Second)
		wg.Wait()

		//Update the timer
		currentTime := time.Since(startTime)
		for i := 0; i < len(players); i++ {
			if players[i].currentLap < laps {
				players[i].timer = currentTime.String()
				players[i].currentTime = currentTime.Seconds()
			}
		}

		//Draw everything
		printGame()
	}

}

func moveCars(car *Car, ranking chan Car) {
	defer wg.Done()

	//Check again if individual car is finished
	if !car.isFinished {
		currentPositionX := car.positionX
		currentPositionY := car.positionY
		newPositionY := car.positionY

		//Give the car a new speed to check collision
		car.currentSpeed = carAcceleration(car.currentSpeed, car.maxSpeed)
		newPositionX := currentPositionX + car.currentSpeed
		mu.Lock()
		newPositionX, newPositionY = checkCollision(currentPositionX, currentPositionY, newPositionX)
		car.currentSpeed = newPositionX - car.positionX
		mu.Unlock()

		//Check if the car is out of the grid, completing a lap
		if newPositionX > (len(racingTrack[0]) - 1) {
			if car.currentLap < laps {
				car.currentLap++
			}
			newPositionX = newPositionX % (len(racingTrack[0]) - 1)
			if car.currentLap == laps {
				car.isFinished = true
				if car.isFinished {
					mu.Lock()
					racingTrack[car.positionY][car.positionX] = 0
					mu.Unlock()
					car.positionX = -1
					car.positionY = -1
					car.rank = (car.currentLap * len(racingTrack[0]))
					ranking <- *car
					return

				}

			}
		}
		mu.Lock()
		racingTrack[currentPositionY][currentPositionX] = 0
		racingTrack[newPositionY][newPositionX] = car.id
		mu.Unlock()

		car.positionX = newPositionX
		car.positionY = newPositionY

		car.rank = (car.currentLap * len(racingTrack[0])) + newPositionX

	}

}

func carAcceleration(currentSpeed int, maxSpeed int) int {
	//Keep speeding up until max speed
	if currentSpeed >= 0 && currentSpeed < maxSpeed-3 {
		return currentSpeed + 1
	}
	min := maxSpeed - 3
	max := maxSpeed

	//If at max speed, return a new speed to deaccelerate
	rand.Seed(time.Now().UnixNano())
	return (rand.Intn(max-min) + min)

}

func checkCollision(currentPositionX int, currentPositionY int, newPositionX int) (int, int) {
	newPositionY := currentPositionY
	for i := currentPositionX; i < newPositionX; i++ {

		//If thr new position is ocupated, change lane
		if racingTrack[currentPositionY][(i+1)%(len(racingTrack[0])-1)] != 0 {

			return i, changeLane(i%(len(racingTrack[0])-1), currentPositionY)
		}
	}
	return newPositionX, newPositionY

}

func changeLane(currentPositionX int, currentPositionY int) int {

	//Check the adjacent lanes for a space to move to.
	if currentPositionY > 0 && currentPositionY < (len(racingTrack)-1) {
		if racingTrack[currentPositionY+1][currentPositionX] == 0 {
			return (currentPositionY + 1)
		} else if racingTrack[currentPositionY-1][currentPositionX] == 0 {
			return (currentPositionY - 1)
		}
	} else if currentPositionY == 0 {
		if racingTrack[currentPositionY+1][currentPositionX] == 0 {
			return (currentPositionY + 1)
		}
	} else if currentPositionY == (len((racingTrack)) - 1) {
		if racingTrack[currentPositionY-1][currentPositionX] == 0 {
			return (currentPositionY - 1)
		}
	}
	return currentPositionY
}

func printGame() {

	//Preparations
	c := exec.Command("clear")
	c.Stdout = os.Stdout
	c.Run()

	//Print the racing track, if the place in the board is empty, print -, else print the number saved on that space
	for i := 0; i < len(racingTrack); i++ {
		for j := 0; j < len(racingTrack[i]); j++ {
			if racingTrack[i][j] == 0 {
				fmt.Printf("- ")
			} else {
				fmt.Printf("%d ", racingTrack[i][j])
			}
		}
		fmt.Printf("\n")
	}

	//Calculate the positions of the cards, in order to print them
	calculatePositions()

}

func allPlayersFinished(ranking chan Car) bool {
	maxLaps := 0
	for i := 0; i < len(players); i++ {
		if players[i].currentLap == laps {
			maxLaps++

		}
	}
	if maxLaps == len(players) {
		return true
	}
	return false

}

func displayWinners(placements []int) {

	fmt.Printf("\n")
	fmt.Printf("First place: %d\n", placements[0])
	fmt.Printf("Second place: %d\n", placements[1])
	fmt.Printf("Third place: %d\n", placements[2])
}

func calculatePositions() {
	var positionsById []float64
	actualTime := time.Since(startTime).Seconds()
	for i := 0; i < len(players); i++ {
		if players[i].isFinished {
			positionsById = append(positionsById, float64(players[i].rank)*(actualTime-players[i].currentTime))
		} else {
			positionsById = append(positionsById, float64(players[i].rank))
		}

	}
	sort.Float64s(positionsById)
	var realPositions []int
	for i := 0; i < len(positionsById); i++ {
		for j := 0; j < len(players); j++ {

			if players[j].isFinished {
				if positionsById[i] == (float64(players[j].rank) * (actualTime - players[j].currentTime)) {
					realPositions = append(realPositions, j)
				}
			} else {
				if positionsById[i] == (float64(players[j].rank)) {
					realPositions = append(realPositions, j)
				}
			}

		}
	}
	realPositions = removeDuplicates(realPositions)
	var positionCounter int = 1
	for i := len(players) - 1; i >= 0; i-- {

		transitTime := players[realPositions[i]].timer
		speed := players[realPositions[i]].currentSpeed
		playerID := realPositions[i]
		playerLaps := players[realPositions[i]].currentLap

		if (playerLaps + 1) > laps {
			playerLaps = laps
		} else {
			playerLaps++
		}
		fmt.Printf("%d°, Racer %d Laps: %d/%d SPEED: %d  Timer: %s\n", positionCounter, playerID+1, playerLaps, laps, speed, transitTime)
		positionCounter++

	}

}

func removeDuplicates(intSlice []int) []int {
	keys := make(map[int]bool)
	list := []int{}
	for _, entry := range intSlice {
		if _, value := keys[entry]; !value {
			keys[entry] = true
			list = append(list, entry)
		}
	}
	return list
}

func main() {

	//Default flags for program usage
	var lapsFlag = flag.Int("laps", 1, "Number of laps for the current race")
	var numberRacers = flag.Int("racers", 3, "How many racers")
	flag.Parse()

	//Setup parameters for the race
	laps = *lapsFlag
	ranking := make(chan Car, *numberRacers)
	if *numberRacers > 50 {
		fmt.Printf("Not enough space for more than 50 racers")
		*numberRacers = 50
	}

	//Whole race
	createRacers(*numberRacers)
	play(ranking)
	close(ranking)

	//Add and print placements
	var placements []int
	for car := range ranking {
		placements = append(placements, car.id)
	}
	displayWinners(placements)

}
