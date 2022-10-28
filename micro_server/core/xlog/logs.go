package xlog

import (
	"fmt"
	"log"
	"os"
)

const callerDepth = 4

// Must checks if err is nil, otherwise logs the error and exits.
func Must(err error) {
	if err == nil {
		return
	}

	msg := err.Error()
	log.Print(msg)
	//getWriter().Severe(msg)
	os.Exit(1)
}

func Info(err string) {
	fmt.Print(err)
}

func Infof(err string, a ...any) {
	fmt.Printf(err, a)
}

func Error(err error) {
	fmt.Print(err)
}

func Errorf(err string, a ...any) {
	fmt.Printf(err, a)
}

func Close() {

}
