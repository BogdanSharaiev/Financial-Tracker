# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Financial_Tracker_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Financial_Tracker_autogen.dir\\ParseCache.txt"
  "Financial_Tracker_autogen"
  )
endif()
