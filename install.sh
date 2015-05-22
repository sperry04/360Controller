#!/bin/bash

echo -e "\nCopying kexts to /Library/Extensions/...";
sudo cp -R build/Release/360Controller.kext /Library/Extensions/
sudo cp -R build/Release/WirelessGamingReceiver.kext /Library/Extensions/
sudo cp -R build/Release/Wireless360Controller.kext /Library/Extensions/

echo -e "\nInstalling 360Controller...";
sudo kextutil -v 6 /Library/Extensions/360Controller.kext

echo -e "\nInstalling WirelessGamingReceiver...";
sudo kextutil -v 6 /Library/Extensions/WirelessGamingReceiver.kext

echo -e "\nInstalling Wireless360Controller...";
sudo kextutil -v 6 /Library/Extensions/Wireless360Controller.kext

