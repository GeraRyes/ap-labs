package main

import (
	"fmt"
	"os"
)

func main() {
  args := os.Args[1:]
  if (len(args)==0){
    fmt.Println("Error")
  }else{
    for i := 0; i < len(args); i++ {
		fmt.Print(args[i]+" ")
	  }
	  fmt.Println("is in advanced programming")
  }
}
  
