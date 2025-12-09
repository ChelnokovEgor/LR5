package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
)

const (
	ZONES_COUNT = 2
	RACKS_PER_ZONE = 6
	SECTIONS_PER_RACK = 4
	SHELVES_PER_SECTION = 4
	SHELVES_CAPACITY = 10
	ZONE_CAPACITY = RACKS_PER_ZONE * SECTIONS_PER_RACK * SHELVES_PER_SECTION * SHELVES_CAPACITY
	TOTAL_CAPACITY = ZONE_CAPACITY * ZONES_COUNT
)

func makeAddress(zone byte, rack, section, shelf int) string {
	return fmt.Sprintf("%c%d%d%d", zone, rack, section, shelf)
}

func isValidAddress(addr string) bool {
	if len(addr) != 4 {
		return false
	}
	z := addr[0]
	if z != 'A' && z != 'B' {
		return false
	}
	for i := 1; i <= 3; i++ {
		if !('1' <= addr[i] && addr[i] <= '9') {
			return false
		}
	}
	r := int(addr[1] - '0')
	s := int(addr[2] - '0')
	sh := int(addr[3] - '0')
	return r >= 1 && r <= RACKS_PER_ZONE &&
		s >= 1 && s <= SECTIONS_PER_RACK &&
		sh >= 1 && sh <= SHELVES_PER_SECTION
}

func initWarehouse(productMap map[string]string, countMap map[string]int) {
	zones := "AB"
	for _, z := range zones {
		for rack := 1; rack <= RACKS_PER_ZONE; rack++ {
			for sec := 1; sec <= SECTIONS_PER_RACK; sec++ {
				for shelf := 1; shelf <= SHELVES_PER_SECTION; shelf++ {
					addr := makeAddress(byte(z), rack, sec, shelf)
					productMap[addr] = ""
					countMap[addr] = 0
				}
			}
		}
	}
}

func warehouseAdd(productMap map[string]string, countMap map[string]int, tokens []string) {
	if len(tokens) < 4 {
		fmt.Println("Ошибка: Недостаточно аргументов. Формат: ADD <товар> <кол-во> <адрес>")
		return
	}
	product := tokens[1]
	addr := tokens[3]
	count, err := strconv.Atoi(tokens[2])
	if err != nil {
		fmt.Println("Ошибка: Некорректное количество.")
		return
	}
	if count <= 0 {
		fmt.Println("Ошибка: Количество должно быть > 0.")
		return
	}
	if !isValidAddress(addr) {
		fmt.Printf("Ошибка: Некорректный адрес '%s'. Правила: <AB> <1-6> <1-4> <1-4>\n", addr)
		return
	}
	currentProduct := productMap[addr]
	currentCount := countMap[addr]
	if currentProduct != "" && currentProduct != product {
		fmt.Printf("Ошибка: В ячейке %s уже лежит '%s'. Нельзя положить '%s'.\n", addr, currentProduct, product)
		return
	}
	if currentCount+count > SHELVES_CAPACITY {
		free := SHELVES_CAPACITY - currentCount
		fmt.Printf("Ошибка: В ячейке '%s' только %dсвободных мест.\n", addr, free)
		return
	}
	productMap[addr] = product
	countMap[addr] = count
	fmt.Printf("Добавлено %d шт. '%s' в %s.\n", count, product, addr)
}

func warehousePop(productMap map[string]string, countMap map[string]int, tokens []string) {
	if len(tokens) < 4 {
		fmt.Println("Ошибка: Недостаточно аргументов. Формат: REMOVE <товар> <кол-во> <адрес>")
		return
	}
	addr := tokens[3]
	count, err := strconv.Atoi(tokens[2])
	if err != nil {
		fmt.Println("Ошибка: Некорректное количество.")
		return
	}
	if count <= 0 {
		fmt.Println("Ошибка: Количество должно быть > 0.")
		return
	}
	if !isValidAddress(addr) {
		fmt.Printf("Ошибка: Некорректный адрес '%s'. Правила: <AB> <1-6> <1-4> <1-4>\n", addr)
		return
	}
	currentProduct := productMap[addr]
	currentCount := countMap[addr]
	product := tokens[1] 
	if currentProduct != product {
		fmt.Printf("Ошибка: В ячейке %s лежит '%s', а не '%s'.\n", addr, currentProduct, product)
		return
	}
	if count > currentCount {
		fmt.Printf("Ошибка: В ячейке %s только %d шт.\n", addr, currentCount)
		return
	}
	currentCount -= count
	if currentCount == 0 {
		currentProduct = ""
		productMap[addr] = ""
	}
	countMap[addr] = currentCount 
	fmt.Printf("Удалено %d шт. '%s' из %s.\n", count, product, addr)
}

func warehouseInfo(productMap map[string]string, countMap map[string]int) {
	totalUsed := 0
	zoneAUsed := 0
	zoneBUsed := 0
	var occupied, empty []string

	for addr := range productMap {
		count := countMap[addr]
		totalUsed += count
		if addr[0] == 'A' {
			zoneAUsed += count
		} else if addr[0] == 'B' {
			zoneBUsed += count
		}
		if count > 0 {
			occupied = append(occupied, addr)
		} else {
			empty = append(empty, addr)
		}
	}

	fmt.Printf("Общая загрузка склада: %.2f%%\n", 100*float64(totalUsed)/float64(TOTAL_CAPACITY))
	fmt.Printf("Загрузка зоны A: %.2f%%\n", 100*float64(zoneAUsed)/float64(ZONE_CAPACITY))
	fmt.Printf("Загрузка зоны B: %.2f%%\n", 100*float64(zoneBUsed)/float64(ZONE_CAPACITY))

	fmt.Println("Содержание занятых ячеек:")
	for _, addr := range occupied {
		fmt.Printf("%s: %s (%d)\n", addr, productMap[addr], countMap[addr])
	}

	fmt.Println("Пустые ячейки:")
	for i, addr := range empty {
		if i > 0 {
			fmt.Print(", ")
		}
		fmt.Print(addr)
	}
	if len(empty) > 0 {
		fmt.Println()
	}
}

func warehouse() {
	productMap := make(map[string]string)
	countMap := make(map[string]int)
	initWarehouse(productMap, countMap)

	scanner := bufio.NewScanner(os.Stdin)
	scanner.Split(bufio.ScanLines)

	fmt.Println("Команды")
	fmt.Println("ADD <товар> <кол-во> <адрес>")
	fmt.Println("REMOVE <товар> <кол-во> <адрес>")
	fmt.Println("INFO")
	fmt.Println("EXIT")

	for {
		if !scanner.Scan() {
			break
		}
		line := scanner.Text()
		if line == "" {
			continue
		}
		var tokens []string
		var token string
		for _, ch := range line {
			if ch == ' ' {
				if token != "" {
					tokens = append(tokens, token)
					token = ""
				}
			} else {
				token += string(ch)
			}
		}
		if token != "" {
			tokens = append(tokens, token)
		}
		if len(tokens) == 0 {
			continue
		}

		cmd := tokens[0]
		switch cmd {
		case "ADD":
			warehouseAdd(productMap, countMap, tokens)
		case "REMOVE":
			warehousePop(productMap, countMap, tokens)
		case "INFO":
			warehouseInfo(productMap, countMap)
		case "EXIT":
			return
		default:
			fmt.Printf("Неизвестная команда: %s\n", cmd)
		}
	}
}
