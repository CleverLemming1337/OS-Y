#!/bin/bash
if grub-file --is-x86-multiboot os-y.bin; then
  echo "Verification successful."
else
  echo "Verification failed."
  exit 1
fi