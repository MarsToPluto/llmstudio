# CMakeLists.txt for llmstudio UI Project

cmake_minimum_required(VERSION 3.16)

project(llmstudio LANGUAGES CXX)

# --- Standard Settings ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON) # Allows #include "mainwindow.h" etc. from src/main.cpp

# --- Find Necessary Packages ---

# --- Qt ---
# Option 1: Hardcoded Path (Adjust to your system) - Simpler for personal setup
set(Qt6_DIR "C:/Qt/6.8.3/mingw_64/lib/cmake/Qt6")

# Option 2: Let CMake find it (Requires Qt directory in CMAKE_PREFIX_PATH or PATH) - More portable
# If using this option, you might need to configure with:
# cmake .. -DCMAKE_PREFIX_PATH="C:/Qt/6.8.3/mingw_64"
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui Concurrent) # Concurrent might be useful for responsive UI
message(STATUS "Using Qt6 Widgets from: ${Qt6_Widgets_DIR}") # Verify correct Qt found

# --- Qt Specific Settings ---
set(CMAKE_AUTOMOC ON)   # Enable automatic Meta-Object Compiler processing
set(CMAKE_AUTOUIC ON)   # Enable automatic User Interface Compiler processing
set(CMAKE_AUTORCC ON)   # Enable automatic Resource Compiler processing

# --- Define the Main Executable ---
# Assumes your UI code is in main.cpp, mainwindow.cpp, mainwindow.h etc. within the 'src' directory
add_executable(llmstudio WIN32 # WIN32 hides the console window on Windows
    src/main.cpp
    src/mainwindow.cpp
    # Add any other .cpp files from your 'src' directory here
)
message(STATUS "Defined executable llmstudio")

# --- Include Directories ---
target_include_directories(llmstudio PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src          # To find your own headers (mainwindow.h etc.)
)

# --- Link Libraries ---
target_link_libraries(llmstudio PRIVATE
    # Qt Modules
    Qt6::Widgets
    Qt6::Gui
    Qt6::Core
    Qt6::Concurrent # Keep if doing background tasks, remove if UI is purely synchronous
)
message(STATUS "Linked libraries for llmstudio")

# --- Deployment (Windows Specific) ---
if(WIN32)
    message(STATUS "Configuring Windows deployment steps...")

    # --- Find windeployqt ---
    # Try finding it within the Qt installation used for building
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt
                 HINTS ${Qt6_INSTALL_PREFIX}/bin ENV PATH ${CMAKE_PREFIX_PATH}/bin
                 DOC "Path to windeployqt executable")

    if(NOT WINDEPLOYQT_EXECUTABLE)
        message(WARNING "windeployqt not found! Deployment of Qt DLLs will be skipped. "
                        "Ensure the Qt bin directory (e.g., C:/Qt/6.x.y/mingw_64/bin) is in your system PATH "
                        "or set CMAKE_PREFIX_PATH correctly.")
    else()
        message(STATUS "Found windeployqt: ${WINDEPLOYQT_EXECUTABLE}")

        # --- Run windeployqt ---
        # Copies necessary Qt DLLs, plugins, translations etc.
        add_custom_command(TARGET llmstudio POST_BUILD
            COMMAND ${WINDEPLOYQT_EXECUTABLE}
                --verbose 1                     # More output during deployment
                #--no-translations              # Uncomment if you don't need Qt translations
                #--no-opengl-sw                 # Uncomment if you don't need the software OpenGL fallback
                --plugindir "$<TARGET_FILE_DIR:llmstudio>/plugins" # Standard plugins subdir
                "$<TARGET_FILE:llmstudio>"       # The executable to deploy
            COMMENT "Running windeployqt to gather Qt dependencies..."
            VERBATIM # Important for commands with special characters
        )
        message(STATUS "Added windeployqt post-build command")
    endif()

    # --- Copy 'binaries' Directory ---
    # Copies the contents of the './binaries' folder from the source tree
    # to the build output directory alongside the executable.
    # Create a 'binaries' directory in your project root if you need this.
    set(SOURCE_BINARIES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/binaries")
    set(DEST_BINARIES_DIR "$<TARGET_FILE_DIR:llmstudio>") # Copy directly into exe dir

    if(EXISTS ${SOURCE_BINARIES_DIR})
         add_custom_command(TARGET llmstudio POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different # CMake's cross-platform copy
                "${SOURCE_BINARIES_DIR}"  # Source directory
                "${DEST_BINARIES_DIR}"    # Destination directory (executable's directory)
            COMMENT "Copying 'binaries' directory contents to build output..."
            VERBATIM
        )
        message(STATUS "Added 'binaries' directory copy post-build command")
    else()
        message(WARNING "Source 'binaries' directory not found at ${SOURCE_BINARIES_DIR}. Skipping copy.")
    endif()

    # --- Copy 'models' Directory (Example - Keep, Remove, or Rename as needed) ---
    # Copies your 'models' folder from the source tree to the build output directory
    # Useful for assets, default data, etc. Rename or remove if not applicable.
    set(SOURCE_MODELS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/models")
    set(DEST_MODELS_DIR "$<TARGET_FILE_DIR:llmstudio>/models")

    if(EXISTS ${SOURCE_MODELS_DIR})
         add_custom_command(TARGET llmstudio POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
                "${SOURCE_MODELS_DIR}"
                "${DEST_MODELS_DIR}"
            COMMENT "Copying 'models' directory to build output..."
            VERBATIM
        )
        message(STATUS "Added 'models' directory copy post-build command")
    else()
        message(INFO "Optional 'models' directory not found at ${SOURCE_MODELS_DIR}. Skipping copy.")
    endif()

endif() # End of WIN32 deployment section


# --- Source Groups (Optional: For IDE organization like Visual Studio/Qt Creator) ---
# Assumes standard MainWindow structure in src/
source_group("Source Files" FILES
    src/main.cpp
    src/mainwindow.cpp
    # Add other .cpp files from src/ here
)
source_group("Header Files" FILES
    src/mainwindow.h
    # Add other .h files from src/ here
)
# If you use UI files (.ui) or Resource files (.qrc)
# source_group("UI Files" FILES src/mainwindow.ui)
# source_group("Resource Files" FILES resources.qrc)

message(STATUS "CMake configuration finished for llmstudio")