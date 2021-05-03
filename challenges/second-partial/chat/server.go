// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 254.
//!+

// Chat is a server that lets clients chat with each other.
package main

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"os"
	"strings"
	"time"
)

//!+broadcaster
type clientChannel chan<- string // an outgoing message channel
type client struct {
	channel     clientChannel
	username    string
	ip          string
	conn        net.Conn
	isAdmin     bool
	loggingTime string
}

var serverHeader string = "irc-server > "

var (
	entering = make(chan client)               //Entering chat channel
	leaving  = make(chan clientChannel)        //Leaving chat channel
	clients  = make(map[clientChannel]*client) //Map of all clients connected to the server in a channel
	messages = make(chan string)               //Incoming messages channel
)

func broadcaster() {
	for {
		select {
		case msg := <-messages: //CASE: MESSAGE WAS SENT
			//Broadcasts messages to all the clients
			for cli := range clients {
				cli <- msg
			}

		case cli := <-entering: //CASE: JOINING SERVER
			if len(clients) == 0 {
				cli.isAdmin = true //If it's the first to be saved in the clients list, it becomes admin
			}
			clients[cli.channel] = &cli //Saves the client on the client's map based on their channel

		case cli := <-leaving: //CASE: LEAVING THE SERVER
			if clients[cli].isAdmin {

				//Aquí el codigo para hacer  nuevo ADMIN

			}
			delete(clients, cli) //Delete the client from the clients map
			close(cli)           //Closes the client channel
		}
	}
}

func becomeAdmin(who string, cli clientChannel) { //In case an admin, it transform the next user into an admin
	fmt.Println(serverHeader + "[" + who + "] is the new ADMIN")
	clients[cli].isAdmin = true //The current user get promoted to ADMIN with this boolean
	cli <- serverHeader + "You're the new IRC Server ADMIN"
}

//!+handleConn
func handleConn(conn net.Conn) {

	var username = false //Validation for repeated username

	input := bufio.NewScanner(conn) //Connection reader
	input.Scan()                    //Input reader
	who := input.Text()             //who equals the username that was written

	for _, newClient := range clients { //This for evaluates every existing user, searching for a matching username, if it's found, the connection is closed because you can't repeat them
		if newClient.username == who {
			username = true //The username was repeated
			fmt.Fprintln(conn, "Repeated username, try another one")
			conn.Close()
			break
		}
	}

	if !username { //This only executes if the username is unique

		//ch is only for the client
		//messages is for everyone

		ch := make(chan string) //Channel for sent messages

		go clientWriter(conn, ch)
		ip := conn.RemoteAddr().String() //Add connection to ip

		ch <- serverHeader + "Welcome to the Simple IRC Server"
		ch <- serverHeader + "Your user [" + who + "] is successfully logged"
		fmt.Println(serverHeader + "New connected user [" + who + "]")
		messages <- serverHeader + "New connected user [" + who + "]"

		entering <- client{ch, who, ip, conn, false, time.Now().Format("2006-01-02 15:04:05")} //Sent the client to the broadcast function and add it to the map

		if clients[ch].isAdmin == true { //If the client is ADMIN
			ch <- serverHeader + "Congrats, you were the first user."
			becomeAdmin(who, ch)
		}

		for input.Scan() { //Were the user can input their massages
			msg := input.Text()
			if msg != "" { //If the message isn't blank

				msgStrings := strings.Split(msg, " ") //Separate the first word, looking for a command
				switch msgStrings[0] {

				case "/users": //Connected users command
					for _, client := range clients {
						ch <- serverHeader + client.username + " - connected since " + client.loggingTime
					}

				case "/msg": //it send a direct message to the specified user
					if len(msgStrings) < 3 {
						ch <- serverHeader + "Error, not enough parameters, please format you message like: /msg <user> <msg>"
					} else {
						var userExists = false
						for _, client := range clients {
							if client.username == msgStrings[1] {
								userExists = true
								client.channel <- who + " > " + msg[6+len(client.username):]
								break
							}
						}
						if !userExists {
							ch <- serverHeader + "Target user does not exist"
						}
					}

				case "/time": //Get the server's local time

					location, _ := time.LoadLocation("America/Mexico_City")
					locationString := location.String()
					ch <- serverHeader + "Local Time: " + locationString + " " + time.Now().Format("3:04PM")

				case "/user": //Get user specific information
					if len(msgStrings) != 2 {
						ch <- serverHeader + "Error, incorrect number of parameters, please format your message like: /user <user>"
					} else {
						var userExists = false
						for _, client := range clients {
							if client.username == msgStrings[1] {
								userExists = true
								ch <- serverHeader + "username: " + client.username + ", IP: " + client.ip + " Connected since: " + client.loggingTime
								break
							}
						}
						if !userExists {
							ch <- serverHeader + "Target user does not exist"
						}
					}

				case "/kick": //Kick user command
					if clients[ch].isAdmin { //Only the ADMIN can kick other users
						if len(msgStrings) != 2 {
							ch <- serverHeader + "Error, incorrect number of parameters, please format your message like: /kick <user>"
						} else {
							var userExists = false
							for _, client := range clients {
								if client.username == msgStrings[1] {
									userExists = true
									client.channel <- serverHeader + "You're kicked from this channel"
									leaving <- client.channel //The kicked user's channel is sent for termination
									messages <- serverHeader + client.username + " has been kicked from the channel"
									fmt.Println(serverHeader + client.username + " has been kicked from the channel")

									client.conn.Close() //The connection is closed
									break
								}
							}
							if !userExists {
								ch <- serverHeader + "Target user not found"
							}
						}
					} else {
						ch <- serverHeader + "ADMIN only command"
					}
				default: //Common messages
					messages <- who + " > " + msg
				}
			}
		}
		if clients[ch] != nil { //If someone leaves
			leaving <- ch
			messages <- serverHeader + "[" + who + "] left the channel"
			fmt.Println(serverHeader + "[" + who + "] left the channel")

			conn.Close()
		}
	}
}

func clientWriter(conn net.Conn, ch <-chan string) { //This is how we print messages on each client
	for msg := range ch {
		fmt.Fprintln(conn, msg) // NOTE: ignoring network errors
	}
}

func main() {
	if len(os.Args) != 5 {
		log.Fatal("Error entering parameters, please format like: go run server.go -host [host] -port [port]")
	}
	server := os.Args[2] + ":" + os.Args[4]
	listener, err := net.Listen("tcp", server)
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println(serverHeader + "Simple IRC Server started at " + server)
	go broadcaster()
	fmt.Println(serverHeader + "Ready for receiving new clients")
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err)
			continue
		}
		go handleConn(conn)
	}
}
