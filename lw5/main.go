package main

import (
	"fmt"
	"net/smtp"
	"os"
)

func main() {
	args := os.Args
	if len(args) < 5 {
		fmt.Println(
			"Usage: ./smtp to@gmail.com from@gmail.com password keyword\n")
		return
	}

	to := []string{args[1]}
	from := args[2]
	password := args[3]
	keyword := args[4]

	if !IsValidGmail(to[0]) || !IsValidGmail(from) {
		fmt.Println("Invalid gmail!")
		return
	}

	message := GetSmtpMessage(to[0], from, keyword)

	server := smtpServer{host: GmailHost, port: SmtpPort}
	auth := smtp.PlainAuth("", from, password, server.host)
	err := smtp.SendMail(server.Address(), auth, from, to, []byte(message))
	if err != nil {
		fmt.Println(err)
		return
	}

	fmt.Println("Email was sent!")
}
