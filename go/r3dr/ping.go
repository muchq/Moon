package main

import "net/http"

func PingHandler(writer http.ResponseWriter, _ *http.Request) {
	writer.Write([]byte("pong"))
}
