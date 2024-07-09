#!/bin/bash

openssl genrsa -out privkey.pem 2048
openssl req -new -x509 -key privkey.pem -out cert.pem -days 365