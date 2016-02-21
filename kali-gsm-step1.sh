#!/bin/bash

echo "Updating Fresh Kali Linux..."

apt update
apt upgrade -y
apt-get install -y kali-linux-all
apt-get install -y flashplugin-nonfree
update-flashplugin-nonfree --install

echo "Done Updating."

echo "Skipping Updating Device Firmware. (Should Be Done By Hand)"
echo "HackRF Users: Check Out https://github.com/mossmann/hackrf/wiki/Updating-Firmware"

echo "STEP 1: Go to Package Downloader (Applications -> Packages (Blue Arrow)) and search for 'osmo'. Download all packages related to SDR, GSM, or GR-GSM."
echo "STEP 2: In Package Downloader, search for 'talloc' and download ALL packages."

echo "STEP 3: Run kali-gsm-step3.sh for remaining steps."
