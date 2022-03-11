#!/bin/sh
curl --verbose -d @../documents/websiteTooLarge.html -X POST http://localhost:8050/testupload.html  -H 'Expect:'
