cmake_minimum_required(VERSION 3.1...3.16)

project(blockhead)


# Locations of Libraries. UPDATE THIS
set(SDL_LIB_DIR "${CMAKE_CURRENT_LIST_DIR}/lib/SDL2-2.0.12")
set(SDL_IMG_DIR "${CMAKE_CURRENT_LIST_DIR}/lib/SDL2_image-2.0.5")
set(SDL_MIX_DIR "${CMAKE_CURRENT_LIST_DIR}/lib/SDL2_mixer-2.0.4")
set(BOOST_LIB_DIR "${CMAKE_CURRENT_LIST_DIR}/lib/boost_1_62_0")

# Get library Header files
set(INCLUDE_DIRS 
    "${SDL_LIB_DIR}/include/;
    ${SDL_IMG_DIR}/include/;
    ${SDL_MIX_DIR}/include/;
    ${BOOST_LIB_DIR}"
)

# Get Library files
set(LIBRARIES
    "${SDL_LIB_DIR}/lib/x64/SDL2.lib;${SDL_LIB_DIR}/lib/x64/SDL2main.lib;${SDL_IMG_DIR}/lib/x64/SDL2_image.lib;${SDL_MIX_DIR}/lib/x64/SDL2_mixer.lib;"
)

# Add library header files
include_directories(${INCLUDE_DIRS})

# Add project header files
include_directories("${CMAKE_CURRENT_LIST_DIR}/include/")

# Get source files
file(GLOB SRC
    "${CMAKE_CURRENT_LIST_DIR}/src/*"
)

# Compile SRC
add_executable(blockhead ${SRC})

# Link SDL2 libs
target_link_libraries(blockhead ${LIBRARIES})

# Move assets to binary dir
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/res"
        DESTINATION "${CMAKE_CURRENT_BINARY_DIR}"
)

# Move SDL2 dll to binary dir
file(GLOB SDL_LIB_DLLS
    "${SDL_LIB_DIR}/lib/x64/*.dll"
)

file(COPY ${SDL_LIB_DLLS}
        DESTINATION "${CMAKE_CURRENT_BINARY_DIR}"
)

# Move SDL img dlls to binary dir
file(GLOB SDL_IMG_DLLS
    "${SDL_IMG_DIR}/lib/x64/*.dll"
)

file(COPY ${SDL_IMG_DLLS}
        DESTINATION "${CMAKE_CURRENT_BINARY_DIR}"
)