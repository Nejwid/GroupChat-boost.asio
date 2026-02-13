cmake_minimum_required(VERSION 3.14)
project(BoostAsioServer LANGUAGES CXX)

# ----------------------------------------
# Ustawienia MSVC dla debug info
# ----------------------------------------
if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
      "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# ----------------------------------------
# C++20 - asio ma funkcje ktore lepiej zeby byly implementowane w cpp20+
# ----------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ----------------------------------------
# Pliki źródłowe serwera/klienta
# ----------------------------------------
# Serwer
file(GLOB_RECURSE SERVER_SRC
    src/server/*.cpp
)
if(SERVER_SRC)
    add_executable(AsioServer ${SERVER_SRC})
    target_include_directories(AsioServer PRIVATE libs/asio-1.36.0/include)
    target_compile_definitions(AsioServer PRIVATE ASIO_STANDALONE)
endif()

# Klient
file(GLOB_RECURSE CLIENT_SRC
    src/client/*.cpp
)
if(CLIENT_SRC)
    add_executable(AsioClient ${CLIENT_SRC})
    target_include_directories(AsioClient PRIVATE libs/asio-1.36.0/include)
    target_compile_definitions(AsioClient PRIVATE ASIO_STANDALONE)
endif()

# ----------------------------------------
# Testy z GTest w katalogu libs/googletest
# ----------------------------------------
file(GLOB_RECURSE TESTS test/*.cpp)

if(TESTS)

    # Włącz testowanie
    enable_testing()

    # Wyłącz instalację GTest (niepotrzebne w projekcie lokalnym)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # Dodaj googletest jako subdirectory
    add_subdirectory(libs/googletest-1.17.0)

    # Tworzymy executable z testów
    add_executable(UnitTests ${TESTS})

    # Linkowanie GTest
    target_link_libraries(UnitTests
        PRIVATE
        GTest::gtest
        GTest::gtest_main
    )

    # Rejestracja testów w CTest
    include(GoogleTest)
    gtest_discover_tests(UnitTests)

endif()
