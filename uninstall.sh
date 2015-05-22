#!/bin/bash

echo -e "\nUninstalling Wireless360Controller...";
sudo kextunload -v 6 /Library/Extensions/Wireless360Controller.kext

echo -e "\nUninstalling WirelessGamingReceiver...";
sudo kextunload -v 6 /Library/Extensions/WirelessGamingReceiver.kext

echo -e "\nUninstalling 360Controller...";
sudo kextunload -v 6 /Library/Extensions/360Controller.kext

echo -e "\nDeleting kexts from /Library/Extensions/...";
sudo rm -rf /Library/Extensions/Wireless360Controller.kext
sudo rm -rf /Library/Extensions/WirelessGamingReceiver.kext
sudo rm -rf /Library/Extensions/360Controller.kext

