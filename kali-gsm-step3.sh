#!/bin/bash

echo "Installing HackRF Libraries"

apt-get install -y hackrf libhackrf-dev libhackrf0

echo "Installing Prerequisites"

apt-get install -y git-core autoconf automake libtool g++ python-dev swig libpcap0.8-dev

echo "Installing Gnuradio"

apt-get install -y gnuradio gnuradio-dev gr-osmosdr gr-osmosdr

echo "Installing Boost and other Prerequisites"

apt-get install -y git cmake libboost-all-dev libcppunit-dev swig doxygen liblog4cpp5-dev python-scipy

apt-get install -y build-essential libtool shtool autoconf automake git-core pkg-config make gcc libpcsclite-dev

echo "Cloning GR-GSM"

git clone https://github.com/ptrkrysik/gr-gsm.git

cd gr-gsm

echo "Cloning Libosmoscore"

git clone git://git.osmocom.org/libosmocore.git

echo "Building Libosmocore"

cd libosmocore

autoreconf -i

./configure

make

make install

ldconfig -i

cd ..

echo "Building GR-GSM"

mkdir build

cd build

cmake ..

make

make install

ldconfig

cd ../..

echo "Installing Gnuradio Configuration"

cat >/etc/gnuradio/config.conf <<EOF
[grc]
local_blocks_path=/usr/local/share/gnuradio/grc/blocks
EOF

echo "Downloading Both Versions of Kalibrate (HackRF and RTL-SDR)"

git clone https://github.com/scateu/kalibrate-hackrf.git

cd kalibrate-hackrf

./bootstrap

./configure

make

cd ..

git clone https://github.com/steve-m/kalibrate-rtl.git

cd kalibrate-rtl

./bootstrap

./configure

make

cd ..

echo ""
echo "For a HackRF, run the following command:"
echo "  cd kalibrate-hackrf && make install"
echo ""
echo "For a RTL-SDR, run the following command:"
echo "  cd kalibrate-rtl && make install"
echo ""
echo "Then Run 'kal -s GSM850 -g 50 -l 50' to search GSM 850 (for example) for GSM towers"
echo "Once you have towers detected, run 'gqrx' and search those frequencies for the tell-tale GSM signal and write down the frequency where it occurs"
echo "From here your system should be able to detect, record, play back, and decode/decrypt GSM signals"
echo "I had to modify airprobe_rtlsdr.py to include frequencies below 925 Mhz, just do a search and replace for 925e6 to 800e6"
echo "Example command using a tower in my area:"
echo "   airprobe_rtlsdr.py -f 879783900 -s 1e6"
echo "and run wireshark to see packets: 'wireshark -k -Y 'gsmtap && !icmp' -i lo'"

echo "Read the entire article for more information:"
echo "  https://docs.google.com/document/d/1E_LQZ0xTs697L-0QsANQ7sDzc09QhO_7q4ue_WXezI0/pub"
