package main

import (
	"bufio"
	"fmt"
	"math/rand"
	"os"
	"strconv"
	"time"
)

type pair struct {
	first  int
	second []string
}

type visitor struct {
	first  string
	second int
}

func sortByTime(visitors []visitor) {
	n := len(visitors)
	for i := 0; i < n-1; i++ {
		swapped := false
		for j := 0; j < n-1-i; j++ {
			if visitors[j].second < visitors[j+1].second {
				visitors[j], visitors[j+1] = visitors[j+1], visitors[j]
				swapped = true
			}
		}
		if !swapped {
			break
		}
	}
}

func sim() {
	fmt.Println("Команды")
	fmt.Println("ENQUEUE <время>")
	fmt.Println("DISTRIBUTE")

	rand.Seed(time.Now().UnixNano())

	fmt.Print("Введите кол-во окон\n ")
	var windowsCount int
	fmt.Scanln(&windowsCount)

	windows := make([]pair, windowsCount)
	for i := range windows {
		windows[i].first = 0
		windows[i].second = []string{}
	}

	var visitors []visitor
	var ticket_counter = 1

	scanner := bufio.NewScanner(os.Stdin)
	scanner.Split(bufio.ScanWords)

READ_LOOP:
	for {
		if !scanner.Scan() {
			break
		}
		cmd := scanner.Text()

		switch cmd {
		case "ENQUEUE":
			if !scanner.Scan() {
				break
			}
			durationStr := scanner.Text()
			duration, err := strconv.Atoi(durationStr)
			if err != nil || duration < 0 {
				continue
			}
			ticket_num := rand.Intn(1000)
			ticket := "T" + strconv.Itoa(ticket_num)
			visitors = append(visitors, visitor{ticket, duration})
			fmt.Println(ticket)
			ticket_counter++

		case "DISTRIBUTE":
			break READ_LOOP

		default:
			fmt.Printf("Неизвестная команда: %s\n", cmd)
		}
	}

	sortByTime(visitors)

	for _, visitor := range visitors {
		minTime := int(^uint(0) >> 1) // INT_MAX emulation
		bestWindow := 0
		for i, window := range windows {
			if window.first < minTime {
				minTime = window.first
				bestWindow = i
			}
		}
		windows[bestWindow].first += visitor.second
		windows[bestWindow].second = append(windows[bestWindow].second, visitor.first)
	}

	for i := 0; i < windowsCount; i++ {
		fmt.Printf("Окно %d (%d минут): ", i+1, windows[i].first)
		if len(windows[i].second) > 0 {
			for j, ticket := range windows[i].second {
				if j > 0 {
					fmt.Print(", ")
				}
				fmt.Print(ticket)
			}
		}
		fmt.Println()
	}
}
