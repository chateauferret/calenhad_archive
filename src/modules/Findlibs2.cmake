# - Try to find LibS2
# Once done this will define
#  LIBS2_FOUND - System has LibXml2
#  LIBS2_INCLUDE_DIRS - The LibXml2 include directories
#  LIBS2_LIBRARIES - The libraries needed to use LibXml2
#  LIBS2_DEFINITIONS - Compiler switches required for using LibXml2

find_package(PkgConfig)
pkg_check_modules(PC_LIBS2 QUIET libs2)
set(LIBS2_DEFINITIONS ${PC_LIBS2_CFLAGS_OTHER})

find_path(LIBS2_INCLUDE_DIR NAMES s2/_fp_contract_off.h)

find_library(LIBS2_LIBRARY NAMES s2 libs2
        HINTS ${PC_LIBS2_LIBDIR} ${PC_LIBS2_LIBRARY_DIRS} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(libs2  DEFAULT_MSG
        LIBS2_LIBRARY LIBS2_INCLUDE_DIR)

mark_as_advanced(LIBS2_INCLUDE_DIR LIBS2_LIBRARY )

set(LIBS2_LIBRARIES ${LIBS2_LIBRARY} )
set(LIBS_INCLUDE_DIRS ${LIBS2_INCLUDE_DIR} )
