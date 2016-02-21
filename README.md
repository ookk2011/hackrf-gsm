# GSM Decoding/Decrypting Using HackRF

The included scripts are intended to update/upgrade a fresh Kali Rolling Edition install to allow gr-gsm, gnuradio, and associated tools to run correctly.  This is not as easy as it sounds, as I've not been able to get gr-gsm to run correctly on any version of Linux otherwise!  By running these scripts and following the instructions, you'll end up with an install of Kali that can run airprobe, gr-gsm sample apps, and decode any GSM message.

To decrypt GSM messages you'll still need the Kc of the messages you'd like to decrypt, which involved using Kraken, an ATI GPU or your CPU, and about 2 TB of rainbow tables, available at https://srlabs.de/decrypting_gsm/ .  However, I've included a patched version of kraken that compiles on this version of Kali (including pre-compiled binaries) so that's another step taken care of.

There seems to be a useful tool to handle most of these steps for you called pytacle, but I've not actually used it (only read the source) so take that for what you will.

# Example commands

Examples are given in the included scripts.  But once you have everything installed and a HackRF (or RTL-SDR) connected, you should be able to run:

     kal -s GSM850 -g 50 -l 50

To search for GSM towers in the 850 band in your area (Verizon and AT&T use this band in the US, for example).

     airprobe_rtlsdr.py -f 879783900 -s 1e6

Will start decoding a tower located at ~879 MHz, a tower in my area.  Open up wireshark to see the packets in the control channel timeslot 0:

     wireshark -k -Y 'gsmtap && !icmp' -i lo

Of course, you may want to capture packets for later review and actual decrypting/decoding of other timeslots, so you could use:

     airprobe_rtlsdr_capture.py -f 879783900 -s 1e6 -g 50 -c ~/out.cfile

To capture a cfile of data (apparently capturing a burst file uses less space, but I'm not sure) and the command

     airprobe_decode.py -m BCCH -t 0 -c ~/out.cfile -f 879783900 -s 1e6

To decode the same channel 0 data as before, however, you can indicate different timeslots, the A5/1 key (if you know it), etc.  Try --help to see all the options.

GR-GSM includes a newer version of these tools in the gr-gsm/apps/ directory called grgsm_livemon/grgsm_decode/grgsm_scanner and includes a gnuradio-companion flowgraph for them, these work as well (if not better) than the airprobe versions, so use whichever you like.

# Licensing

Each subproject has it's own license that governs itself.  My own code is released under the MIT license.
