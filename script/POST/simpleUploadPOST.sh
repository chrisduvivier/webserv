#!/bin/sh
curl --verbose -d @../documents/succes.html -X POST http://localhost:8050/testupload.html  -H 'Expect:'
