// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 254.
//!+

// Chat is a server that lets clients chat with each other.

/*
REFERENCES
	https://codingornot.com/13-go-to-go-maps-en-golang
	https://www.golangprograms.com/get-current-date-and-time-in-various-format-in-golang.html
	https://www.geeksforgeeks.org/time-time-location-function-in-golang-with-examples/


*/
//Made by Ulises Bojorquez Ortiz, A01114716
package main

import (
	"bufio"
	"flag"
	"fmt"
	"log"
	"net"
	"os"
	"strconv"
	"strings"
	"time"
)

type client chan<- string // an outgoing message channel

type Client struct {
	username   string
	address    string
	channel    client
	connection net.Conn
	admin      bool
	kicked     bool
	login      string
}

type GeneralMessage struct {
	message string
	from    string
	to      string
}

var (
	entering           = make(chan *Client)        //entering channel
	leaving            = make(chan *Client)        //leaving channel
	messagesToClients  = make(chan GeneralMessage) // all incoming client messages
	clientsConnected   = make(map[string]*Client)  // all connected clients in the server
	usersCommand       = make(chan *Client)        //userscommand channel
	privMessageCommand = make(chan GeneralMessage) //private message channel
	userCommand        = make(chan GeneralMessage) //usercommand channel
	kickCommand        = make(chan GeneralMessage) //kickcommand channel
	isAdmin            = false                     //global admin var
	isFirst            = true                      //global first client entering var
)

var host = flag.String("host", "localhost", "host address")
var port = flag.Int("port", 8000, "port number")

func broadcaster() {
	for {
		select {
		case msg := <-messagesToClients:
			// Broadcast incoming message to all
			// clients' outgoing message channels.
			sendMessageToAllClients(msg.from, msg.from+" > "+msg.message)

		case cli := <-entering:
			clientsConnected[cli.username] = cli
			sendServerSelf("New connected user [" + cli.username + "]")
			sendServerToAllClients(cli, "New connected user ["+cli.username+"]")
		case cli := <-leaving:
			removeClient(cli)
		case cli := <-usersCommand:
			for c := range clientsConnected {
				sendServerToClient(cli, c+" - connected since "+clientsConnected[c].login+"\n")
			}
		case msg := <-privMessageCommand:
			clientsConnected[msg.to].channel <- clientsConnected[msg.from].username + " > " + msg.message
		case msg := <-userCommand:
			for cli := range clientsConnected {
				if cli == msg.from {
					sendServerToClient(clientsConnected[msg.from], "username "+clientsConnected[msg.to].username+
						", IP: "+clientsConnected[msg.to].address+
						" Connected since "+clientsConnected[msg.to].login+"\n")
					break
				}
			}
		case msg := <-kickCommand:
			sendServerToClient(clientsConnected[msg.to], "You're kicked from this channel\n")
			sendServerToClient(clientsConnected[msg.to], "You cannot send more messages\n")
			sendMessageToAllClients("irc-server", "irc-server > ["+msg.to+"] was kicked from channel\n")
			sendServerSelf("[" + msg.to + "] was kicked")
			clientsConnected[msg.to].kicked = true
			removeClient(clientsConnected[msg.to])
		}
	}
}

func handleConn(conn net.Conn) {
	ch := make(chan string)   // outgoing client messages
	go clientWriter(conn, ch) //gorutine that writes everything to the client

	//scan the username from the client
	name := bufio.NewScanner(conn)
	name.Scan()
	//ge the address from the client
	addr := conn.RemoteAddr().String()
	currentTime := time.Now()
	var who = Client{
		username:   name.Text(),
		address:    addr,
		channel:    ch,
		connection: conn,
		admin:      false,
		kicked:     false,
		login:      currentTime.Format("2006-01-02 15:04:05"),
	}
	sendServerToClient(&who, "Welcome to the Simple IRC Server\n")
	sendServerToClient(&who, "Your user ["+who.username+"] is successfully logged\n")
	entering <- &who
	assignAdmin(&who)
	input := bufio.NewScanner(conn) //this is the input that receives the server from the client
	for input.Scan() {
		inputMessage := input.Text()
		if strings.Contains(inputMessage, "/") {
			arrayMessage := strings.Split(inputMessage, " ")
			if strings.Contains(arrayMessage[0], "/") {
				manageSubCommands(&who, arrayMessage)
			} else {
				var general = GeneralMessage{
					message: inputMessage,
					from:    who.username,
				}
				messagesToClients <- general
			}
		} else {
			var general = GeneralMessage{
				message: inputMessage,
				from:    who.username,
			}
			messagesToClients <- general
		}
	}
	if who.kicked == false {
		leaving <- &who
		conn.Close()
	}
}

func assignAdmin(who *Client) {
	if isAdmin == false {
		if isFirst == true {
			sendServerToClient(who, "Congrats, you were the first user.\n")
			sendServerToClient(who, "You're the new IRC Server ADMIN\n")
			sendServerSelf("[" + who.username + "] was promoted as the channel ADMIN")
			isFirst = false
		} else {
			sendServerToClient(who, "You're the new IRC Server ADMIN\n")
			sendServerSelf("[" + who.username + "] was promoted as the channel ADMIN")
		}
		isAdmin = true
		who.admin = true
	}
}

func removeClient(who *Client) {
	sendServerSelf("[" + who.username + "] left")
	sendServerToAllClients(who, "["+who.username+"] left channel\n")
	delete(clientsConnected, who.username)
	close(who.channel)
	who.connection.Close()
	if who.admin == true && len(clientsConnected) > 1 {
		for cli := range clientsConnected {
			if cli != who.username {
				isAdmin = false
				assignAdmin(clientsConnected[cli])
				break
			}
		}
	}
}

func manageSubCommands(who *Client, array []string) {
	switch array[0] {
	case "/users":
		if len(array) > 1 {
			sendServerToClient(who, "Write the command with no spaces before \n")
			break
		} else {
			usersCommand <- who
		}
	case "/msg":
		if len(array) > 2 {
			cliExist := "false"
			for cli := range clientsConnected {
				if cli == array[1] {
					cliExist = "true"
					break
				}
			}
			if cliExist == "true" {
				var texto string
				for i := 2; i < len(array); i++ {
					texto += array[i] + " "
				}
				texto += "\n"
				var general = GeneralMessage{
					message: texto,
					from:    who.username,
					to:      array[1],
				}
				if general.from != general.to {
					privMessageCommand <- general
				} else {
					sendServerToClient(who, "You can't send a message to yourself\n")
				}
			} else {
				sendServerToClient(who, "This user does not exist\n")
			}
		} else {
			sendServerToClient(who, "Some parameters are missing\n")
		}
	case "/time":
		if len(array) > 1 {
			sendServerToClient(who, "Write the command with no spaces before \n")
			break
		} else {
			now := time.Now()
			sendServerToClient(who, "Local Time: "+now.Location().String()+" "+now.Format("15:04")+"\n")
		}
	case "/user":
		if len(array) == 2 {
			cliExist := "false"
			for cli := range clientsConnected {
				if cli == array[1] {
					cliExist = "true"
					break
				}
			}
			if cliExist == "true" {
				var general = GeneralMessage{
					from: who.username,
					to:   array[1],
				}
				userCommand <- general
			} else {
				sendServerToClient(who, "This user does not exist\n")
			}
		} else {
			sendServerToClient(who, "Some parameters are wrong\n")
		}
	case "/kick":
		if who.admin == true {
			if len(array) == 2 {
				cliExist := "false"
				for cli := range clientsConnected {
					if cli == array[1] {
						cliExist = "true"
						break
					}
				}
				if cliExist == "true" {
					var general = GeneralMessage{
						to: array[1],
					}
					kickCommand <- general
				} else {
					sendServerToClient(who, "This user does not exist\n")
				}
			} else {
				sendServerToClient(who, "Some parameters are wrong\n")
			}
		} else {
			sendServerToClient(who, "No permission to execute this command\n")
		}
	default:
		sendServerToClient(who, "This command is not recognized\n")
	}
}

func clientWriter(conn net.Conn, ch <-chan string) {
	for msg := range ch {
		fmt.Fprintln(conn, msg) // NOTE: ignoring network errors
	}
}

func sendMessageToAllClients(who string, message string) {
	for cli := range clientsConnected {
		if cli != who {
			clientsConnected[cli].channel <- message
		}
	}
}

func sendServerToAllClients(who *Client, message string) {
	for key := range clientsConnected {
		if clientsConnected[key].username != who.username {
			clientsConnected[key].channel <- "irc-server > " + message
		}
	}
}

func sendServerToClient(who *Client, message string) {
	who.channel <- "irc-server > " + message
}

func sendServerSelf(message string) {
	fmt.Println("irc-server > " + message)
}

func main() {
	connection := os.Args[1:]  //all the arguments
	argSize := len(connection) //length of the command-line arguments

	if argSize != 4 {
		fmt.Println("Some parameters are missing.")
	} else if connection[0] != "-host" || connection[2] != "-port" {
		fmt.Println("Unrecognized flags.")
	} else {
		flag.Parse()
		hostport := *host + ":" + strconv.Itoa(*port)
		listener, err := net.Listen("tcp", hostport)
		if err != nil {
			log.Fatal(err)
		}
		sendServerSelf("Simple IRC Server started at " + hostport)
		sendServerSelf("Ready for receiving new clients")
		go broadcaster()
		for {
			conn, err := listener.Accept()
			if err != nil {
				log.Print(err)
				continue
			}
			go handleConn(conn)
		}
	}
}
