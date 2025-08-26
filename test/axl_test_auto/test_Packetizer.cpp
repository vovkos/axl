//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#include "pch.h"
#include "test.h"

namespace {

//..............................................................................

class MyPacketizer: public sl::Packetizer<MyPacketizer> {
public:
	void
	onPacket(
		const void* p,
		size_t size
	) {
		printf("packet received: %s\n", p);
	}
};

void
run() {
	printf("test_Packetizer::run()\n");

	MyPacketizer packetizer;

	char data[] = "papapaapak:\xf\0\0\0pizda ivanovna";

	for (size_t i = 0; i < 64; i++) {
		char* p = data;
		size_t size = sizeof(data);

		size_t j = 0;
		for (; size; j++) {
			size_t chunkSize = rand() % (sizeof(data) + 1);
			if (chunkSize > size)
				chunkSize = size;

			packetizer.write(p, chunkSize);
			p += chunkSize;
			size -= chunkSize;
		}

		printf("complete (%d, %d chunks).\n", i, j);
	}

	sl::String s = "dsjsdljsdkl";
	s.clear();

	printf("done.\n");
}

//..............................................................................

ADD_TEST_CASE("test_Packetizer", run)

}
