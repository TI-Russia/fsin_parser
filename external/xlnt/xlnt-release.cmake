#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "xlnt" for configuration "Release"
set_property(TARGET xlnt APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(xlnt PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C;CXX"
  IMPORTED_LOCATION_RELEASE "C:/Sources/xlnt/build/installed/lib/xlnt.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS xlnt )
list(APPEND _IMPORT_CHECK_FILES_FOR_xlnt "C:/Sources/xlnt/build/installed/lib/xlnt.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
