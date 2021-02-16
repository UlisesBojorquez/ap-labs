package main

import (
	"fmt"
	"os"
	"strings"
)

func main() {

	args := os.Args[1:]
	var tam int = len(args)

	result := strings.Join(args, " ")

	if tam >= 1 {
		fmt.Println("Hello", result, ", Welcome to the Jungle")
	} else {
		fmt.Println("An Error Occur!!")
	}

}