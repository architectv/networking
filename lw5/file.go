package main

import (
	"io/ioutil"
	"strings"
)

type File struct {
	Name    string
	Content string
}

func GetFiles(dir string, keyword string) []File {
	files, err := ioutil.ReadDir(dir)
	if err != nil {
		panic(err)
	}

	keyword = strings.ToLower(keyword)

	var keyFiles []File

	for _, f := range files {
		if f.IsDir() {
			continue
		}

		name := f.Name()
		content, err := ioutil.ReadFile(dir + name)
		if err != nil {
			panic(err)
		}

		text := string(content)
		checkName := strings.Contains(strings.ToLower(name), keyword)
		checkContent := strings.Contains(strings.ToLower(text), keyword)
		if checkName || checkContent {
			keyFiles = append(keyFiles, File{
				Name:    name,
				Content: text,
			})
		}
	}

	return keyFiles
}
