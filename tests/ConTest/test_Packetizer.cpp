#include "pch.h"

using namespace axl;

namespace test_Packetizer {

//.............................................................................

class CMyPacketizer: public rtl::CPacketizerT <CMyPacketizer>
{
public:
	void
	OnPacket (
		const void* p,
		size_t Size
		)
	{
		printf ("packet received: %s\n", p);
	}
};

void
Run ()
{
	printf ("test_Packetizer::Run ()\n");

	CMyPacketizer Packetizer;

	char Data [] = "papapaapak:\xf\0\0\0pizda ivanovna";

	srand (::GetTickCount ());

	for (size_t i = 0; i < 64; i++)
	{
		char* p = Data;
		size_t Size = sizeof (Data);

		size_t j = 0;
		for (; Size; j++)
		{
			size_t ChunkSize = rand () % (sizeof (Data) + 1);
			if (ChunkSize > Size)
				ChunkSize = Size;

			Packetizer.Write (p, ChunkSize);
			p += ChunkSize;
			Size -= ChunkSize;
		}

		printf ("complete (%d, %d chunks).\n", i, j);
	}
	
	rtl::CString s = "dsjsdljsdkl";
	s.Clear ();

	printf ("done.\n");
}

//.............................................................................

} // namespace test_Packetizer
