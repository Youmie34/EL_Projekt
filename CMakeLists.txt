cmake_minimum_required(VERSION 3.10)

# Projektname und Version
project(EmbeddedLinuxProjekt VERSION 1.0)

# C++ Standard festlegen
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include-Verzeichnisse hinzufügen
include_directories(${PROJECT_SOURCE_DIR}/IPC/header)

# Alle Quelldateien auflisten
set(SOURCES
    main.c
    IPC/src/prozess_1.cpp
    IPC/src/prozess_2.cpp
    IPC/src/prozess_3.cpp
    IPC/src/helper.cpp
)

# Executable erstellen
add_executable(EmbeddedLinuxProjekt ${SOURCES})

# Erforderliche Bibliotheken hinzufügen
target_link_libraries(EmbeddedLinuxProjekt pthread gpiod)