package main

import (
	"fmt"
	"math"
	"math/rand"
	"os"
	"sort"
	"strconv"
	"time"
)

type Point struct{ x, y float64 }

// traditional function
func Distance(p, q Point) float64 {
	return math.Hypot(q.x-p.x, q.y-p.y)
}

// same thing, but as a method of the Point type
func (p Point) Distance(q Point) float64 {
	return math.Hypot(q.x-p.x, q.y-p.y)
}

func (p Point) X() float64 {
	return p.x
}

func (p Point) Y() float64 {
	return p.y
}

//!-point

//!+path

// A Path is a journey connecting the points with straight lines.
type Path []Point

// Distance returns the distance traveled along the path.
func (path Path) Distance() float64 {
	sum := 0.0
	for i := 0; i < len(path); i++ {
		if i < len(path)-1 {
			sum += path[i].Distance(path[i+1])

		} else {
			sum += path[i].Distance(path[0])
		}
	}
	return sum
}

//!-path

func GeneratePoint() Point {
	XPoint := rand.Float64()*(100-(-100)) - 100
	YPoint := rand.Float64()*(100-(-100)) - 100

	return Point{XPoint, YPoint}
}

type Orientation Path

func (o Orientation) Len() int {
	return len(o)
}
func (o Orientation) Swap(i, j int) {
	o[i], o[j] = o[j], o[i]
}
func (o Orientation) Less(i, j int) bool {
	return math.Atan2(o[i].Y(), o[i].X()) < math.Atan2(o[j].Y(), o[j].X())
}

func CreatePoints(path Path, sides int) Path {

	for i := 0; i < sides; i++ {
		path[i] = GeneratePoint()
	}

	sort.Sort(Orientation(path))

	return path
}

func main() {

	var arguments []string = os.Args[1:]
	sides, _ := strconv.Atoi(arguments[0])

	if len(arguments) == 1 && sides > 2 {
		rand.Seed(time.Now().UnixNano())
		//Create array length
		var path Path = make(Path, sides)
		//Generate Points
		finalPath := CreatePoints(path, sides)

		fmt.Printf("- Generating a [%v] sides figure\n", sides)
		fmt.Println("- Figure's vertices")
		for i := range finalPath {
			fmt.Printf("   - ( %.2f, %.2f)\n", finalPath[i].X(), finalPath[i].Y())
		}
		fmt.Println("- Figure's perimeter")
		for i := 0; i < len(finalPath); i++ {
			if i < len(finalPath)-1 {
				if i == 0 {
					fmt.Printf("   - %.2f + ", finalPath[i].Distance(finalPath[i+1]))
				} else {
					fmt.Printf(" %.2f + ", finalPath[i].Distance(finalPath[i+1]))
				}

			} else {
				fmt.Printf("%.2f = ", finalPath[i].Distance(finalPath[0]))
			}
		}
		fmt.Printf("%.2f\n", finalPath.Distance())
	} else {
		fmt.Println("ERROR! Some parameters are wrong!")
	}

}
