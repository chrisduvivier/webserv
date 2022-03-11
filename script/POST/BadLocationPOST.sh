#!/bin/sh
curl --verbose -d @../documents/website.html -X POST http://localhost:8050/tmp/testupload.html  -H 'Expect:'
