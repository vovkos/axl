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

#pragma once

#define _AXL_PY_STDIO_H

#include "axl_py_Type.h"
#include "axl_rc_Buf.h"

namespace axl {
namespace py {

//..............................................................................

class StdioStream: public rc::RefCount {
public:
	virtual
	bool
	write(
		const void* p,
		size_t size
	) = 0;

	virtual
	bool
	flush() {
		return true;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct StdoutObject: PyObject {
	rc::Ptr<StdioStream> m_stream;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class StdioTypeObject: public TypeObject {
public:
	StdioTypeObject();

protected:
	static
	void
	dealloc(PyObject* self) {
		sl::destruct(&((StdoutObject*)self)->m_stream);
		self->ob_type->tp_free(self);
	}

	static
	PyObject*
	write(
		PyObject* self,
		PyObject* args
	);

	static
	PyObject*
	flush(
		PyObject* self,
		PyObject* args
	) {
		bool result = ((StdoutObject*)self)->m_stream->flush();
		return result ? Py_None : py::failCFunction();
	}
};

//..............................................................................

bool
createStdio(
	PyObject** object,
	const rc::Ptr<StdioStream>& stream
);

inline
py::Object
createStdio(const rc::Ptr<StdioStream>& stream) {
	py::Object original;
	createStdio(&original, stream);
	return original;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
setStdio(
	PyObject** original,
	const sl::StringRef& name,
	PyObject* object
);

inline
py::Object
setStdio(
	const sl::StringRef& name,
	PyObject* object
) {
	py::Object original;
	setStdio(&original, name, object);
	return original;
}

inline
bool
setStdout(
	PyObject** original,
	PyObject* object
) {
	return setStdio(original, AXL_STR("stdout"), object);
}

inline
py::Object
setStdout(PyObject* object) {
	py::Object original;
	setStdout(&original, object);
	return original;
}

inline
bool
setStderr(
	PyObject** original,
	PyObject* object
) {
	return setStdio(original, AXL_STR("stderr"), object);
}

inline
py::Object
setStderr(PyObject* object) {
	py::Object original;
	setStderr(&original, object);
	return original;
}

//..............................................................................

} // namespace py
} // namespace axl
