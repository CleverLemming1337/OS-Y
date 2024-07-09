#!/bin/bash

echo "Step 4: Signing EFI"

sudo apt install sbsigntool

mv main.efi unsigned.efi
sbsign --key privkey.pem --cert cert.pem --output signed.efi unsigned.efi