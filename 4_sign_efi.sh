#!/bin/bash

sudo apt install sbsigntool

sbsign --key privkey.pem --cert cert.pem --output signed.efi main.efi