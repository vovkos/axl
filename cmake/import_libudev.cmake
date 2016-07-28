#..............................................................................

unset (_DIR)

find_path (
	_DIR
	NAMES libudev.h
	)

if (_DIR)
	set (LIBUDEV_FOUND TRUE)
else ()
	set (LIBUDEV_FOUND FALSE)
endif ()

unset (_DIR CACHE)

#..............................................................................
