#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "xlnt" for configuration "Debug"
set_property(TARGET xlnt APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(xlnt PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C;CXX"
  IMPORTED_LOCATION_DEBUG "C:/Sources/xlnt/build/installed/lib/xlntd.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS xlnt )
list(APPEND _IMPORT_CHECK_FILES_FOR_xlnt "C:/Sources/xlnt/build/installed/lib/xlntd.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
