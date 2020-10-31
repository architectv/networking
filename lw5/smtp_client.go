package main

import (
	"fmt"
	"regexp"
)

func IsValidGmail(gmail string) bool {
	re := regexp.MustCompile(`^[a-z0-9._%+\-]+@gmail.com$`)
	return re.MatchString(gmail)
}

func GetSmtpMessage(to, from, keyword string) string {
	files := GetFiles(ClientDir, keyword)

	message := fmt.Sprintf("To: %s\n", to)
	message += fmt.Sprintf("From: %s\n", from)
	message += fmt.Sprintf("Subject: Files By Keyword \"%s\"\n", keyword)
	message += "Mime-Version: 1.0\n"
	message += fmt.Sprintf(
		"Content-Type: multipart/mixed; boundary=\"%s\"\n\n", Delimeter)

	for _, file := range files {
		message += fmt.Sprintf("--%s\n", Delimeter)
		message += "Content-Type: text/plain; charset=\"utf-8\"\n"
		message += "Content-Transfer-Encoding: 8bit\n"
		message += fmt.Sprintf(
			"Content-Disposition: attachment; filename=\"%s\"\n\n", file.Name)
		message += file.Content
		message += "\n"
	}

	return message
}
