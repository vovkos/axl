#..............................................................................

if (EXISTS ${WIX_BIN_DIR}/candle.exe)
	set (WIX_FOUND TRUE)
else ()
	set (WIX_FOUND FALSE)
endif ()

#..............................................................................