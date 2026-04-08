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
#include "axl_py_Stdio.h"
#include "axl_py_Tuple.h"
#include "axl_py_Module.h"

namespace axl {
namespace py {

//..............................................................................

StdioTypeObject::StdioTypeObject() {
	static PyMethodDef methods[] = {
		{
			"write",
			write,
			METH_VARARGS
		},
		{
			"flush",
			flush,
			METH_NOARGS
		},
		{ NULL }
	};

	tp_name = "axl.py.Stdout";
	tp_basicsize = sizeof(StdoutObject);
	tp_new = PyType_GenericNew;
	tp_methods = methods;
	tp_dealloc = dealloc;
	tp_flags = Py_TPFLAGS_DEFAULT;

	bool result = ready();
	ASSERT(result);
}

PyObject*
StdioTypeObject::write(
	PyObject* self,
	PyObject* args0
) {
	const void* p;
	Py_ssize_t size;

	py::TupleBorrowed args = args0;
	bool result = args.parseArgs("s#", &p, &size);
	if (!result)
		return py::failCFunction(PyExc_TypeError);

	result = ((StdoutObject*)self)->m_stream->write(p, size);
	return result ? Py_None : py::failCFunction();
}

//..............................................................................

bool
createStdio(
	PyObject** resultObject,
	const rc::Ptr<StdioStream>& stream
) {
	static py::StdioTypeObject type;
	py::Object object = py::ObjectBorrowed(type).call();
	if (!object)
		return false;

	StdoutObject* stdoutObject = (StdoutObject*)object.detach();
	sl::construct(&stdoutObject->m_stream, stream);
	*resultObject = stdoutObject;
	return true;
}

bool
setStdio(
	PyObject** original,
	const sl::StringRef& name,
	PyObject* object
) {
	py::Module sys;

	return
		importModule(&sys, AXL_STR("sys")) &&
		sys.getAttr(original, name) &&
		sys.setAttr(name, object);
}

//..............................................................................

} // namespace py
} // namespace axl
