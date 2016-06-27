#Update repos first just in case
sudo apt-get update

#Install other packages from the Raspbian Repository
sudo apt-get -y install libcurl4-openssl-dev || { echo "Failed to install libcurl4-openssl-dev" && exit; }
sudo apt-get -y install i2c-tools || { echo "Failed to install i2c-tools" && exit; }
sudo apt-get -y install libi2c-dev || { echo "Failed to install libi2c-dev" && exit; }

#Install Broadcom GPIO driver
cd BCM2835
tar zxvf bcm2835-1.50.tar.gz
cd bcm2835-1.50
./configure
make
sudo make check
sudo make install
cd ..
cd ..

#At this point, everything should be installed and working after reboot
echo "Sensorian C Interface dependencies should now be setup. Please reboot!"