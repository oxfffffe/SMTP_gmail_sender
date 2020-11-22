# SMTP Client written in pure C

## How to start?

First of all, compile files with `gcc client.c main.c -lcurl -o Mail`.

Next, you should to visit `https://www.google.com/settings/security/lesssecureapps` and
turn on Access for less secure apps button.

## Usage
```
./Mail \
	--from 			<your e-mail>@gmail.com \
	--to   			<receiver e-mail>@gmail.com \
	--password 		<your password> \
	--subject 		... \
	--text 			... \
	--sender 		... \
```

To get more info, please type `./Mail --help`.