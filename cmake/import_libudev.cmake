#..............................................................................

find_path (
	_DIR
	NAMES libudev.h
	HINTS /usr/include
	)

if (_DIR)
	set (LIBUDEV_FOUND TRUE)
else ()
	set (LIBUDEV_FOUND FALSE)
endif ()

unset (_DIR CACHE)

#..............................................................................
