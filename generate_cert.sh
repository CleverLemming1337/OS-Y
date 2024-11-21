#!/bin/bash

openssl genrsa -out privkey.pem 2048
openssl req -new -x509 -key build/privkey.pem -out build/cert.pem -days 365