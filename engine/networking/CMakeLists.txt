#--------------------------------------------------------------------------
# networking
#--------------------------------------------------------------------------

SET(files_networking
	console.h
	console.cc
	dead_reck.h
	dead_reck.cc
	laser.h
	laser.cc
	network.h
	network.cc
	spaceship.h
	spaceship.cc
	)
SOURCE_GROUP("networking" FILES ${files_networking})
	
SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(networking STATIC ${files_networking} ${files_pch})
TARGET_PCH(networking ../)
ADD_DEPENDENCIES(networking glew enet)
TARGET_LINK_LIBRARIES(networking PUBLIC engine exts glew enet soloud)
