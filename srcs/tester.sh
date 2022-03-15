#!/bin/bash

# Reset
Color_Off='\033[0m'       # Text Reset

# Regular Colors
Black='\033[0;30m'        # Black
Red='\033[0;31m'          # Red
Green='\033[0;32m'        # Green
Yellow='\033[0;33m'       # Yellow
Blue='\033[0;34m'         # Blue
Purple='\033[0;35m'       # Purple
Cyan='\033[0;36m'         # Cyan
White='\033[0;37m'        # White

# Bold
BBlack='\033[1;30m'       # Black
BRed='\033[1;31m'         # Red
BGreen='\033[1;32m'       # Green
BYellow='\033[1;33m'      # Yellow
BBlue='\033[1;34m'        # Blue
BPurple='\033[1;35m'      # Purple
BCyan='\033[1;36m'        # Cyan
BWhite='\033[1;37m'       # White

# Underline
UBlack='\033[4;30m'       # Black
URed='\033[4;31m'         # Red
UGreen='\033[4;32m'       # Green
UYellow='\033[4;33m'      # Yellow
UBlue='\033[4;34m'        # Blue
UPurple='\033[4;35m'      # Purple
UCyan='\033[4;36m'        # Cyan
UWhite='\033[4;37m'       # White

# Background
On_Black='\033[40m'       # Black
On_Red='\033[41m'         # Red
On_Green='\033[42m'       # Green
On_Yellow='\033[43m'      # Yellow
On_Blue='\033[44m'        # Blue
On_Purple='\033[45m'      # Purple
On_Cyan='\033[46m'        # Cyan
On_White='\033[47m'       # White

# High Intensity
IBlack='\033[0;90m'       # Black
IRed='\033[0;91m'         # Red
IGreen='\033[0;92m'       # Green
IYellow='\033[0;93m'      # Yellow
IBlue='\033[0;94m'        # Blue
IPurple='\033[0;95m'      # Purple
ICyan='\033[0;96m'        # Cyan
IWhite='\033[0;97m'       # White

# Bold High Intensity
BIBlack='\033[1;90m'      # Black
BIRed='\033[1;91m'        # Red
BIGreen='\033[1;92m'      # Green
BIYellow='\033[1;93m'     # Yellow
BIBlue='\033[1;94m'       # Blue
BIPurple='\033[1;95m'     # Purple
BICyan='\033[1;96m'       # Cyan
BIWhite='\033[1;97m'      # White

# High Intensity backgrounds
On_IBlack='\033[0;100m'   # Black
On_IRed='\033[0;101m'     # Red
On_IGreen='\033[0;102m'   # Green
On_IYellow='\033[0;103m'  # Yellow
On_IBlue='\033[0;104m'    # Blue
On_IPurple='\033[0;105m'  # Purple
On_ICyan='\033[0;106m'    # Cyan
On_IWhite='\033[0;107m'   # White

clear

echo -e $Blue
echo "Checking dependencies ... "

if ! command -v tree &> /dev/null
then brew install tree
fi

if ! command -v siege &> /dev/null
then brew install siege
fi
sleep 1

echo "👍"

sleep 1

echo "This is how our working directory looks like ⤵"
echo -e $Purple
sleep 1

tree ./

echo -e $White
echo -e $On_Green
read -n 1 -r -s -p $'Press enter to continue'
echo -e $Color_Off

clear

echo -e $Yellow
echo "Building webserv ..."
echo -e $Color_Off

make re

echo -e $Yellow
echo "Webserv is ready. Please run webser on a different terminal"
echo -e $Color_Off

sleep 3

read -n 1 -r -s -p $'Press enter when ready'
echo -e $Color_Off

clear

############# LAUNCH WEBSERV ##############

echo -e $Blue
echo "Let's do some tests"

sleep 1

############## GET ##############

echo -e $Blue
echo "Opening root location in the browser (GET method)"

sleep 2

open http://127.0.0.1:8050/

sleep 3

echo -e $On_Green
read -n 1 -r -s -p $'Press enter to continue'
echo -e $Color_Off

clear

############## POST ##############

echo -e $Blue
echo "Let's try uploading a file (POST method)"

echo -e $On_Green
read -n 1 -r -s -p $'Press enter to execute'
echo -e $Color_Off

curl --verbose -d @../script/documents/succes.html -X POST http://localhost:8050/testupload.html  -H 'Expect:'

sleep 1
clear

echo -e $Blue
echo "Let's check the uploaded file"

open http://localhost:8050/upload/testupload.html

sleep 1
clear

echo -e $Blue
echo "we can overwrite the file if we send another POST upload on the same PATH"

sleep 1

echo -e $On_Green
read -n 1 -r -s -p $'Press enter to Overwrite testupload.html'
echo -e $Color_Off

curl --verbose -d @../script/documents/website.html -X POST http://localhost:8050/testupload.html  -H 'Expect:'

echo -e $Blue
echo "Let's check the uploaded file"

open http://localhost:8050/upload/testupload.html

sleep 1

echo -e $On_Green
read -n 1 -r -s -p $'Press enter to continue'
echo -e $Color_Off

clear

############## DELETE ##############

echo -e $Blue
echo "You can also delete files inside /upload directory."
echo "We have prepared a file to delete, let's open it."
sleep 1
open http://localhost:8050/upload/deleteMe.html

sleep 1
echo -e $Blue
echo "Let's DELETE the file"

echo -e $On_Green
read -n 1 -r -s -p $'Press enter to continue'
echo -e $Color_Off

clear
curl --verbose -X DELETE http://localhost:8050/upload/deleteMe.html

sleep 1
echo -e $Blue
echo "Going back to the deleted page should get you a 404 error"

echo -e $On_Green
read -n 1 -r -s -p $'Press enter to continue'
echo -e $Color_Off

clear

############## CONFIG ##############

# MULTIPLE PORT
echo -e $Blue
echo "Let's check different PORT"

sleep 1
open http://localhost:8020/

sleep 2
echo -e $On_Green
read -n 1 -r -s -p $'Press enter to continue'
echo -e $Color_Off
clear

# MULTIPLE SERVER NAME
echo -e $Blue
echo "Let's check different SERVER NAME"

sleep 1
curl --resolve localhost:8050:127.0.0.1 localhost:8050

sleep 2
echo -e $On_Green
read -n 1 -r -s -p $'Press enter to continue'
echo -e $Color_Off
clear

# DELETE Without Permission
echo -e $Blue
echo "Trying to DELETE protected pages is FORBIDDEN"

sleep 1
curl --verbose -X DELETE http://localhost:8050/error_pages/default_error.html

sleep 2
echo -e $On_Green
read -n 1 -r -s -p $'Press enter to continue'
echo -e $Color_Off
clear

# Client MAX BODY SIZE
echo -e $Blue
echo "If the REQUEST exceeds CLIENT_MAX_BODY_SIZE, error is returned"

sleep 1
curl --verbose -d @../script/documents/websiteTooLarge.html -X POST http://localhost:8050/testupload.html  -H 'Expect:'

sleep 2
echo -e $On_Green
read -n 1 -r -s -p $'Press enter to continue'
echo -e $Color_Off

clear

# UNKNOWN request
echo -e $Blue
echo "UNKNOWN request is forbidden, let's try PUT"

sleep 1
curl --verbose -X PUT http://localhost:8050/upload/testupload.html

sleep 2
echo -e $On_Green
read -n 1 -r -s -p $'Press enter to continue'
echo -e $Color_Off

clear

############## CGI ##############
echo -e $Blue
echo "Let's test some CGI"

sleep 1
echo "[1st] Openining simple form cgi (GET)"
sleep 1
open http://localhost:8050/simple_form.html
sleep 2
echo -e $On_Green
read -n 1 -r -s -p $'Press enter to continue'
echo -e $Color_Off

echo -e $Blue
echo "[2nd] Openining register cgi (POST)"
sleep 1
open http://localhost:8050/register.html
sleep 2
echo -e $On_Green
read -n 1 -r -s -p $'Press enter to continue'
echo -e $Color_Off

echo -e $Blue
echo "[3rd] Openining env_list cgi (POST)"
sleep 1
open http://localhost:8050/env_list.html
sleep 2
echo -e $On_Green
read -n 1 -r -s -p $'Press enter to continue'
echo -e $Color_Off

############ EXIT ##############

sleep 3

echo -e $On_Red
read -n 1 -r -s -p $'When you are done testing, press enter to kill webserv process'
echo -e $Color_Off

pkill webserv