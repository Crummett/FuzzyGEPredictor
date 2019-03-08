# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
# ADAPTED FROM FUZZYLITE DEMO
cmake_minimum_required (VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

project ("FuzzyGEPredictor")

# Include sub-projects.
add_subdirectory ("FuzzyGEPredictor")

option(FL_BACKTRACE "Provide backtrace information in case of errors" ON)
option(FL_STATIC "Statically link to fuzzylite libraries" ON)

set(BacktraceLibrary)

if (MSVC AND FL_BACKTRACE)
    set(BacktraceLibrary dbghelp)
endif()

if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(FL_DEBUG ON)
else()
    set(FL_DEBUG OFF)
endif()


set(FL_SYSTEM_LIBRARY FALSE) #Finds the library installed in the system
set(FL_LIBRARY_DIR)

if (NOT FL_SYSTEM_LIBRARY)
    #it is possible to find the FuzzyLiteLibrary locally as follows,
    #assuming the submodule of fuzzylite is added to the ${PROJECT_SOURCE_DIR}:
    set(FL_HOME ${PROJECT_SOURCE_DIR}/fuzzylite)
    set(FL_INCLUDE_DIR ${FL_HOME})
    if (FL_DEBUG)
        set(FL_LIBRARY_DIR ${FL_HOME}/bin/debug)
    else()
        set(FL_LIBRARY_DIR ${FL_HOME}/bin/release)
    endif()
    message("Finding FuzzyLiteLibrary locally at ${FL_LIBRARY_DIR}")

    include_directories(${FL_INCLUDE_DIR})
endif()

set(FL_POSTFIX)
if (FL_STATIC)
    set(FL_POSTFIX "-static")
endif()
if (FL_DEBUG)
    set(FL_POSTFIX "${FL_POSTFIX}-debug")
endif()

set(FL_LIBRARY_NAME  fuzzylite${FL_POSTFIX})
find_library (FuzzyLiteLibrary ${FL_LIBRARY_NAME} HINTS ${FL_LIBRARY_DIR})

#we add the definition of the building path to remove it when using macro FL_LOG
add_definitions(-DFL_BUILD_PATH="${CMAKE_SOURCE_DIR}") #used to determine

#we add the sources
include_directories(FuzzyGEPredictor/headers)

file(GLOB SOURCES "FuzzyGEPredictor/src/*")
file(GLOB HEADERS "FuzzyGEPredictor/headers/*")
 
if(MSVC)
#Set compilation flags in Windows
    set(CMAKE_CXX_FLAGS "/W4 /EHsc")
    #Wx: Treat warnings as errors. W4: All warnings
    #http://msdn.microsoft.com/en-us/library/thxezb7y.aspx
    #EHsc: call destructors on __try __catch, and to ignore C4530: C++ exception handler used. Note, unwind semantics are not enabled
endif()
 
	
#we create the binary
add_executable(FuzzyGEPredictor ${SOURCES} ${HEADERS})
if (NOT FL_STATIC)
    target_compile_definitions(FuzzyGEPredictor PRIVATE FL_IMPORT_LIBRARY)
endif()
#linking the fuzzylite library
target_link_libraries (FuzzyGEPredictor ${FuzzyLiteLibrary} ${BacktraceLibrary})
#setting the name of the product
set_target_properties(FuzzyGEPredictor PROPERTIES OUTPUT_NAME FuzzyElections)
#specially for windows
set_target_properties(FuzzyGEPredictor PROPERTIES OUTPUT_NAME FuzzyElections IMPORT_PREFIX tmp-) #To prevent LNK1149 in Windows
#in case of building on debug mode
set_target_properties(FuzzyGEPredictor PROPERTIES DEBUG_POSTFIX d)


message("=====================================")
message("FL_HOME=${FL_HOME}")
message("FL_LIBRARY_NAME=${FL_LIBRARY_NAME}")
message("FuzzyLiteLibrary=${FuzzyLiteLibrary}")
message("")
message("FL_BACKTRACE=${FL_BACKTRACE}")
message("FL_STATIC=${FL_STATIC}")
message("FL_DEBUG=${FL_DEBUG}")
message("CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
message("CMAKE_CXX_COMPILER_ID=${CMAKE_CXX_COMPILER_ID}")
message("CMAKE_CXX_COMPILER_VERSION=${CMAKE_CXX_COMPILER_VERSION}")
message("CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")
message("COMPILE_DEFINITIONS:")
get_directory_property(fl-definitions DIRECTORY ${CMAKE_SOURCE_DIR} COMPILE_DEFINITIONS )
foreach(d ${fl-definitions})
    message( STATUS "Defined: " ${d} )
endforeach()
message("=====================================\n")

