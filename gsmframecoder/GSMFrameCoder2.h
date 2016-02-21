/*
 * GSMFrameCoder2.h
 *
 *  Created on: 09.01.2011
 *      Author: johann
 */

#ifndef GSMFRAMECODER2_H_
#define GSMFRAMECODER2_H_

#include "BitVector.h"
class GSMFrameCoder2 {

	public:

	/**@name FEC signal processing state.  */
	/*//@{
	Parity mBlockCoder;			///< block coder for this channel
	BitVector mI[4];			///< i[][], as per GSM 05.03 2.2
	BitVector mC;				///< c[], as per GSM 05.03 2.2
	BitVector mU;				///< u[], as per GSM 05.03 2.2
	BitVector mD;				///< d[], as per GSM 05.03 2.2
	BitVector mP;				///< p[], as per GSM 05.03 2.2
	//@}*/


	/** Offset from the start of mU to the start of the L2 frame. */
	//virtual unsigned headerOffset() const { return 0; }

	/**
	  Encode u[] to c[].
	  Includes LSB-MSB reversal within each octet.
	*/
	void encode();

	/**
	  Interleave c[] to i[].
	  GSM 05.03 4.1.4.
	*/
	virtual void interleave();

	/** Just encode u[] to c[], interleave and return the interleaved bursts as BitVector[] */
	BitVector* justencode(const BitVector&);

};

#endif /* GSMFRAMECODER2_H_ */
