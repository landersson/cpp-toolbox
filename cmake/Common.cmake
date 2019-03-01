
#message(MY=${CMAKE_SOURCE_DIR})

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin*" )
  include_directories("/opt/local/include/")
  link_directories("/opt/local/lib/")
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux*" )
  include_directories(/usr/include/jsoncpp)
endif()
