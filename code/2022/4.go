package main

import (
 "log"
 "net/http"
 _ "net/http/pprof"
 "time"
)

func main() {
 go func() {
  log.Fatal(http.ListenAndServe(":9999", nil))
 }()

 var data [][]byte
 for {
  data = func1(data)
  time.Sleep(1 * time.Second)
 }
}

func func1(data [][]byte) [][]byte {
 data = func2(data)
 return append(data, make([]byte, 1024*1024)) // alloc 1mb
}

func func2(data [][]byte) [][]byte {
 return append(data, make([]byte, 1024*1024)) // alloc 1mb
 }
