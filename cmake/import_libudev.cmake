#..............................................................................

find_path (
	_DIR
	NAMES libudev.h
	HINTS /usr/include
	)

if (_DIR)
	set (LIBUDEV_FOUND TRUE)
	message (STATUS "Path to LibUdev includes:   ${_DIR}")
else ()
	set (LIBUDEV_FOUND FALSE)
	message (STATUS "LibUdev:                    <not-found>")
endif ()

unset (_DIR CACHE)

#..............................................................................
