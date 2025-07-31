package main

import "C"
import (
	"log"
	"time"
)

func main() {
	// This is a placeholder for the main function.
	// The actual implementation will depend on the specific requirements of the application.
}

//export Execute
func Execute(input *C.char) *C.char {
	goInput := C.GoString(input)
	log.Println("Executing with input:", goInput)
	// This function is a placeholder for the actual implementation.
	// It should contain the logic that needs to be executed when called from C.
	return C.CString("Hello " + goInput + ` from Go!` + "\n")
}

//export ExecuteAsync
func ExecuteAsync(input *C.char) *C.char {
	goInput := C.GoString(input)
	time.Sleep(2 * time.Second) // Simulate some asynchronous work
	log.Println("Executing asynchronously with input:", goInput)
	return C.CString("Hello " + goInput + ` from Go Async!` + "\n")
}
