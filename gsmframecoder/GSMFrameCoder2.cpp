/*
 * GSMFrameCoder.cpp
 *
 *  Created on: 19.12.2010
 *      Author: johann
 */

#include <fstream>

#include "GSMFrameCoder2.h"

//#include "GSMTransfer.h"
#include <string>
#include <iostream>
//#include <stdio.h>
#include <stdlib.h>

using namespace std;

Parity mBlockCoder(0x10004820009ULL, 40, 224); ///< block coder for this channel
BitVector mI[4]; ///< i[][], as per GSM 05.03 2.2
BitVector mC(456); ///< c[], as per GSM 05.03 2.2
BitVector mU(228); ///< u[], as per GSM 05.03 2.2
BitVector mD(mU.head(184)); ///< d[], as per GSM 05.03 2.2
BitVector mP(mU.segment(184, 40)); ///< p[], as per GSM 05.03 2.2*/
ViterbiR2O4 mVCoder;

void encode() {
	// Perform the FEC encoding of GSM 05.03 4.1.2 and 4.1.3

	// GSM 05.03 4.1.2
	// Generate the parity bits.
	mBlockCoder.writeParityWord(mD, mP);
	//OBJLOG(DEEPDEBUG) << "XCCHL1Encoder u[]=" << mU;
	// GSM 05.03 4.1.3
	// Apply the convolutional encoder.
	mU.encode(mVCoder, mC);
	//OBJLOG(DEEPDEBUG) << "XCCHL1Encoder c[]=" << mC;
}

void interleave() {
	// GSM 05.03, 4.1.4.  Verbatim.
	for (int k = 0; k < 456; k++) {
		int B = k % 4;
		int j = 2 * ((49 * k) % 57) + ((k % 8) / 4);
		mI[B][j] = mC[k];
	}
}

BitVector* justencode(const BitVector& frame) {
	frame.copyToSegment(mU, 0);
	//OBJLOG(DEEPDEBUG) << "XCCHL1Encoder d[]=" << mD;
	mD.LSB8MSB();
	//OBJLOG(DEEPDEBUG) << "XCCHL1Encoder d[]=" << mD;
	encode(); // Encode u[] to c[], GSM 05.03 4.1.2 and 4.1.3.
	interleave(); // Interleave c[] to i[][], GSM 05.03 4.1.4.
	return mI;
}

void strip_spaces(char* string) {
	char *p1 = string;
	char *p2 = string;
	p1 = string;
	while (*p1 != 0) {
		if (isspace(*p1)) {
			++p1;
		} else
			*p2++ = *p1++;
	}
	*p2 = 0;
}

const char *htoi(const char *ptr) {
	char ch = *ptr;
	int length = strlen(ptr);
	string value;
	//value = (char*) malloc(length * 4 * sizeof(char));

	while (ch == ' ' || ch == '\t')
		ch = *(++ptr);
	int i;
	for (i = 0; i < length; i++) {
		//const char* bin;
		string bin;
		if (!isspace(ch)) {
			switch (tolower(ch)) {
			case '0':
				bin = "0000";
				break;
			case '1':
				bin = "0001";
				break;
			case '2':
				bin = "0010";
				break;
			case '3':
				bin = "0011";
				break;
			case '4':
				bin = "0100";
				break;
			case '5':
				bin = "0101";
				break;
			case '6':
				bin = "0110";
				break;
			case '7':
				bin = "0111";
				break;
			case '8':
				bin = "1000";
				break;
			case '9':
				bin = "1001";
				break;
			case 'a':
				bin = "1010";
				break;
			case 'b':
				bin = "1011";
				break;
			case 'c':
				bin = "1100";
				break;
			case 'd':
				bin = "1101";
				break;
			case 'e':
				bin = "1110";
				break;
			case 'f':
				bin = "1111";
				break;
			default:
				return value.c_str();
			}
			value.append(bin);
		}
		ch = *(++ptr);
	}
	return value.c_str();
}

/*
 char* xorVals(char* value, char* key) {

 short unsigned int vlen = strlen(value);
 short unsigned int k = 0;
 char retval[vlen];

 for (int v = 0; v < vlen; v++) {
 retval[v] = value[v] ^ key[k];
 }
 return retval;
 }*/

int main(int argc, char *argv[]) {
	bool binary = false;
	//bool doxor = false;
	//int xpos = 3;
	int bpos = 1;
	BitVector framebits;
	char* input;
	input = (char*) malloc(500 * sizeof(int));

	if (argc > 1) {
		for (int i = 1; i <= argc - 1; i++) {
			if (strcmp(argv[i], "-b") == 0) {
				binary = true;
				bpos = 1;
				if (i < argc - 1) {
					if (strcmp(argv[i + 1], "-x") != 0 && strcmp(argv[i + 1],
							"-X") != 0) {
						bpos = i + 1;
					}
				}
			} else {
				strcat(input, argv[i]);

			}
			/*else if (strcmp(argv[i], "-x") == 0) {
			 xpos = i;
			 doxor = true;
			 }*/
		}

		// Set up the interleaving buffers.
		for (int k = 0; k < 4; k++) {
			mI[k] = BitVector(114);
			// Fill with zeros just to make Valgrind happy.
			mI[k].fill(0);
		}

		// zero out u[] to take care of tail fields
		mU.zero();

		//const char *frame;
		//char input[184];
		//char *input;
		//cout << "Insert Frame" << endl;
		//cin >> input;

		//BitVector framebits("0000000101110011001101010000011000100111000000000000001101010000000110001010000000000101111101000000000101011100010101110000001100101011001010110010101100101011001010110010101100101011");

		if (!binary) {
			//todo convert into binary
			cout << "Decoding " << input << endl;
			framebits = BitVector(htoi(input));
		} else {
			cout << "Decoding binary" << endl;
			framebits = BitVector(argv[bpos]);
		}
		BitVector* encframe;

		encframe = justencode(framebits);
		cout << "Encoded Frame, Burst1: " << "\n" << encframe[0] << endl;
		cout << "Encoded Frame, Burst2: " << "\n" << encframe[1] << endl;
		cout << "Encoded Frame, Burst3: " << "\n" << encframe[2] << endl;
		cout << "Encoded Frame, Burst4: " << "\n" << encframe[3] << endl;

		/*	char singlechar[114];
		 if (doxor) {
		 for(unsigned int i=0; i<encframe[0].size(); i++){
		 singlechar[i] = encframe[0][i];
		 cout << "SC= "<< singlechar[i]  << endl;
		 }*/

		/*if (strlen() != strlen(argv[xpos])) {
		 cout << "Cannot XOR, uneven amount of bits" << endl;
		 return 0;
		 }else{
		 //cout << "Xored bursts:" << xorVals(encframe[0].c_str(),argv[xpos]) << endl;
		 //cout << "Xored bursts:" << xorVals(encframe[1],argv[xpos]) << endl;
		 //cout << "Xored bursts:" << xorVals(encframe[2],argv[xpos]) << endl;
		 //cout << "Xored bursts:" << xorVals(encframe[3],argv[xpos]) << endl;
		 }

		 }*/

	} else {
		cout << "Usage:\n" << "gsmframecoder <burst to encode>\n"
				<< "-x <bits> - Xor output with <bits>\n"
				<< "-b <burst to encode> - give the burst in binary representation instead of hex"
				<< endl;
	}
}

