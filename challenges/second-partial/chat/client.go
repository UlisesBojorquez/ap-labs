// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 227.

// Netcat is a simple read/write client for TCP servers.

/*
REFERENCES
https://zetcode.com/golang/flag/
*/
//Made by Ulises Bojorquez Ortiz, A01114716
package main

import (
	"bufio"
	"flag"
	"fmt"
	"io"
	"log"
	"net"
	"os"
)

var username = flag.String("user", "", "client username")
var server = flag.String("server", "localhost:8000", "host:port")

func main() {

	connection := os.Args[1:]  //all the arguments
	argSize := len(connection) //length of the command-line arguments

	if argSize != 4 {
		fmt.Println("Some parameters are incorrect.")
	} else if connection[0] != "-user" || connection[2] != "-server" {
		fmt.Println("Unrecognized flags.")
	} else {
		flag.Parse() // used to execute the command-line parsing.
		conn, err := net.Dial("tcp", *server)
		if err != nil {
			fmt.Printf("No connection established\n")
			log.Fatal(err)
		}
		done := make(chan struct{}) //channel that checks if something happends
		_, e := io.WriteString(conn, *username+"\n")
		if e != nil {
			log.Fatal(err)
		}
		go func() {
			input := bufio.NewScanner(conn)
			for input.Scan() {
				fmt.Print("\n" + input.Text())
				fmt.Print("\n" + *username + " > ")
			}
			io.Copy(os.Stdout, conn) // NOTE: ignoring errors
			os.Exit(0)
			done <- struct{}{} // signal the main goroutine
		}()
		mustCopy(conn, os.Stdin)
		conn.Close()
		<-done // wait for background goroutine to finish
	}
}

//Here we write and send text to the server
func mustCopy(dst io.Writer, src io.Reader) {
	output := bufio.NewScanner(src)
	for output.Scan() {
		if output.Text() == "" {
			fmt.Print(*username + " > ")
			continue
		}
		_, e := io.WriteString(dst, output.Text()+"\n")
		if e != nil {
			fmt.Printf("Connection closed\n")
			return
		}
		fmt.Print(*username + " > ")
	}
}
