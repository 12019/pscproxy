# Try to find libgnutls library. In case of a success the following libraries
# will be set:
# LIBGNUTLS_FOUND - Set to true if libgnutls has been found
# LIBGNUTLS_INCLUDE_DIR - libgnutls include dir
# LIBGNUTLS_LIBRARIES - libgnutls libraries that are supposed to be linked

if(LIBGNUTLS_INCLUDE_DIR AND LIBGNUTLS_LIBRARIES)
	set(LIBGNUTLS_FOUND TRUE)
else(LIBGNUTLS_INCLUDE_DIR AND LIBGNUTLS_LIBRARIES)
	message(STATUS "checking for libgnutls")
	find_path(LIBGNUTLS_INCLUDE_DIR gnutls/gnutls.h
		/include
		/usr/include
		/usr/local/include
		/opt/include
		/opt/local/include)

	if(NOT LIBGNUTLS_INCLUDE_DIR)
		message(STATUS "gnutls includes not fount!!!!")
	endif(NOT LIBGNUTLS_INCLUDE_DIR)
	
	find_library(LIBGNUTLS_LIBRARIES NAMES gnutls
		PATHS
		/lib
		/usr/lib
		/usr/local/lib
		/opt/lib
		/opt/local/lib)
	if(NOT LIBGNUTLS_LIBRARIES)
		message(STATUS "gnutls lib not found!!!!!")
	endif(NOT LIBGNUTLS_LIBRARIES)
	if(LIBGNUTLS_INCLUDE_DIR AND LIBGNUTLS_LIBRARIES)
		set(LIBGNUTLS_FOUND TRUE)
	endif(LIBGNUTLS_INCLUDE_DIR AND LIBGNUTLS_LIBRARIES)
endif(LIBGNUTLS_INCLUDE_DIR AND LIBGNUTLS_LIBRARIES)

