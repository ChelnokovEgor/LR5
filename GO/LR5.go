package main

import "fmt"

func main() {
	var k int
	fmt.Scanln(&k)
	switch k {
	case 1:
		warehouse()
	case 2:
		sim()
	}
}
