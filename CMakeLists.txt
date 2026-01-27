# CMakeList.txt: projekt CMake dla BoostAsioServer
cmake_minimum_required(VERSION 3.8)

# Ustawienia MSVC dla debug info
if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
      "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("BoostAsioServer")


# zbieramy wszystkie pliki z tych lokalizacji zamiast recznie wpiywac 
file(GLOB_RECURSE SOURCES
    server/*.cpp
    client/*.cpp
    *.cpp
)

file(GLOB_RECURSE HEADERS
    server/*.h
    client/*.h
    *.h
)

add_executable(BoostAsioServer ${SOURCES} ${HEADERS})

target_include_directories(BoostAsioServer PRIVATE libs/asio-1.36.0/include)

# Wymuszamy C++20
if(CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET BoostAsioServer PROPERTY CXX_STANDARD 20)
endif()

# TODO: Dodaj testy i zainstaluj elementy docelowe w razie potrzeby.
