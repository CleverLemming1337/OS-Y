#!/bin/bash

echo "Step 4: Signing EFI"

sudo apt install sbsigntool

mv build/main.efi build/unsigned.efi
sbsign --key build/privkey.pem --cert build/cert.pem --output build/main.efi build/unsigned.efi
