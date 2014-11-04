#include "pch.h"

#include <string>

namespace test_Crypto { 

std::string base32encode (const unsigned char* p, size_t size)
{
	static const char map [33] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

	std::string text;

	size_t bits = size * 8;
	size_t byteBitPos = 0;

	bool hasUnfinishedBlock = bits % 5 != 0;

	unsigned char block = 0;
	int blockBitPos = 0;

	for (size_t i = 1; i <= bits; i++)
	{
		bool isBitSet = (*p & (1 << byteBitPos++)) != 0;

		if (isBitSet)
			block |= 1 << blockBitPos;
	
		blockBitPos++;

		if (i % 5 == 0)
		{
			text += map [block];
			block = 0;
			blockBitPos = 0;
		}
	
		if (i % 8 == 0)
		{
			p++;
			byteBitPos = 0;
		}
	}

	if (hasUnfinishedBlock)
		text += map [block];

	return text;
}

std::string base32decode (const unsigned char* p, size_t size)
{
	std::string text;

	unsigned char byte = 0;
	int bitsInByte = 0;

	const unsigned char* end = p + size;
	for (; p < end; p++)
	{
		unsigned char block = 0;

		unsigned char c = *p;
		if (c >= 'A' && c <= 'Z')
			block = c - 'A';
		else if (c >= '2' && c <= '7')
			block = 26 + (c - '2');
		else return ""; // not base32
		
		int bitsToRead = 8 - bitsInByte;
		if (bitsToRead > 5) bitsToRead = 5;
		unsigned char mask = (1 << bitsToRead) - 1;

		byte |= (block & mask) << bitsInByte;
		bitsInByte += bitsToRead;
		
		if (bitsInByte == 8)
		{
			text += byte;

			if (bitsToRead == 5) // all bits from current block read
			{
				byte = 0;
				bitsInByte = 0;
			}
			else // start new byte with remaining bits from current block
			{
				int blockBitsRemaining = 5 - bitsToRead;
				int mask = ((1 << blockBitsRemaining) - 1) << bitsToRead;

				byte = (block & mask) >> bitsToRead;
				bitsInByte = blockBitsRemaining;
			}
		}
	}

	if (bitsInByte) // out of blocks but still a byte with data remaining
		text += byte;

	return text;
}

//.............................................................................

void run ()
{
	char s [] = "hui govno i muravei, MURAVEI GOVNO I HUI!";
	
	rtl::String e1 = enc::Base32Encoding::encode (s,  sizeof (s));
	printf ("axl enc = %s\n", e1.cc ());

	std::string e2 = base32encode ((uchar_t*) s,  sizeof (s));
	printf ("ref enc = %s\n", e2.c_str ());

	rtl::Array <char> x1 = enc::Base32Encoding::decode (e1);
	printf ("axl dec = %s\n", x1.ca ());

	std::string x2 = base32decode ((const uchar_t*) e2.c_str (), e2.size ());
	printf ("ref dec = %s\n", x2.c_str ());

//	ASSERT (memcmp (e1, e2.c_str (), e1.getLength ()) == 0);
	ASSERT (memcmp (x1, x2.c_str (), x1.getCount ()) == 0);

}

//.............................................................................

} // namespace test_Vso
