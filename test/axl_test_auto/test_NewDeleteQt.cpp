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
#include <QObject>

namespace {

//..............................................................................

void run() {
	QObject* parent = new QObject;
	printf("parent: %p\n", parent);
	QObject* child = new QObject(parent);
	printf("child: %p\n", child);
	delete parent;
}

//..............................................................................

ADD_TEST_CASE("test_NewDeleteQt", run)

}
