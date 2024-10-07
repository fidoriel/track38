#Change to directory where wxwidgets is located. 

#git: git clone --recurse-submodules https://github.com/wxWidgets/wxWidgets.git

#git: git clone --recurse-submodules https://github.com/fidoriel/track38.git
#UNIX like wxWidgets
#create build directory Mac: cd wxWidgets; mkdir macbuild; cd macbuild
#create build directory Linux: cd wxWidgets; mkdir gtkbuild; cd gtkbuild

#build instrucions for wxWidgets Platform Linux und macOS: ../configure --disable-shared --disable-sys-libs
#RasPiOS: Deps: sudo apt install libgtk-3-dev
#RasPi: ../configure --disable-shared --disable-sys-libs --with-gtk=3
#compile with: make -j

#change to project directory and: mkdir build; cd build
#cmake corfigure with: cmake ..
#if Linux AppImage should be created cmake .. -D RELEASE=1
#compile with: make -j

#Windows gcc Deprecated
#change build directory Win32: cd build\msw
#build instrucions for wxWidgets Platform Windows MinGW: mingw32-make SHELL=CMD.exe -j4 -f makefile.gcc BUILD=release UNICODE=1 SHARED=0 MONOLITHIC=0 DwxUSE_LIBPNG=builtin DwxUSE_ZLIB=builtin DCMAKE_POSITION_INDEPENDENT_CODE=ON
#mingw-w64 workaround i686 and Version 6.3
#mingw32-make SHELL=CMD.exe -j4 -f makefile.gcc BUILD=release UNICODE=1 SHARED=0 MONOLITHIC=0 DwxUSE_LIBPNG=builtin DwxUSE_ZLIB=builtin DCMAKE_POSITION_INDEPENDENT_CODE=ON LDFLAGS=-Wl,--allow-multiple-definition
#the build command will fail the first and second time. the thrid will be succesful
#mingw32 Compile: mingw32-make -j4

#Windows x64 only MSVC
# nmake /f makefile.vc BUILD=debug SHARED=1 TARGET_CPU=X64
# cmake -G "Visual Studio 16 2019" -A x64 -S .. -B . -Wno-dev
# cmake --build . --config Debug -j 4

#Debug
# cmake .. -DCMAKE_BUILD_TYPE=Debug

#change to project directory and: mkdir build; cd build
#cmake CMD.exe: cmake .. -G "MinGW Makefiles"
#mingw32 Compile: mingw32-make -j4

include(CMakePrintHelpers)

cmake_minimum_required( VERSION 3.18 )
set( CMAKE_BIULD_TYPE "Debug" )
set( CMAKE_CXX_STANDARD 17 )

project( track38 )

set( WXWIDGETS_VERSION_IN_HOME_DIR "wxWidgets" )
set( RELEASE 1 )

message( "CXX Compiler: ${CMAKE_CXX_COMPILER}" )
message( "CXX Compiler ID: ${CMAKE_CXX_COMPILER_ID}" )
message( "CXX Compiler is GNU: ${CMAKE_COMPILER_IS_GNUCXX}" )

# SimpleBLE
add_subdirectory(src/SimpleBLE/ ${CMAKE_BINARY_DIR}/simpleble)
include_directories(${SIMPLEBLE_INCLUDES})

# wxWidgets
set( wxWidgets_SRC_DIR "src/wxWidgets" )
set( wxWidgets_BUILD_DIR "wxWidgets" )

# set system vars
set( IS_WIN false )
set( IS_MAC false )
set( IS_LINUX false )

if( WIN32 )
    message( "Target: Windows32" )
    set( IS_WIN true )
elseif( APPLE )
    message( "Target: Apple" )
    set( IS_MAC true )
    set( wxWidgets_CONFIG_ARGS "--disable-shared --disable-sys-libs")
elseif( CMAKE_SYSTEM_NAME STREQUAL "Linux" )
    message( "Target: Linux" )
    set( IS_LINUX true )
    set( wxWidgets_CONFIG_ARGS "--disable-shared --disable-sys-libs --with-gtk=3")
endif()


if( IS_WIN )
    if( RELEASE )
        set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_NAME}/" )
    endif()

    set( wxWidgets_CONFIG_DIR "${wxWidgets_SRC_DIR}/build/")
    file(MAKE_DIRECTORY ${wxWidgets_CONFIG_DIR})

    set( wxWidgets_LIB_DIR "${wxWidgets_SRC_DIR}/lib/vc_x64_dll/" )
    set (wxWidgets_CONFIGURATION "${wxWidgets_SRC_DIR}/lib/vc_x64_dll/mswud/" )

    find_package(wxWidgets COMPONENTS core base REQUIRED )
    include( ${wxWidgets_USE_FILE})

elseif( IS_MAC OR IS_LINUX )
    file(MAKE_DIRECTORY ${wxWidgets_BUILD_DIR})
    set( wxWidgets_CONFIG_EXECUTABLE "${wxWidgets_BUILD_DIR}/wx-config" )

    if(NOT EXISTS ${wxWidgets_BUILD_DIR}/Makefile)
        message(STATUS "Configuring wxWidgets...")
        execute_process(
            COMMAND  bash ../../src/wxWidgets/configure ${wxWidgets_CONFIG_ARGS}
            RESULT_VARIABLE wxWidgets_CONFIGURE_RESULT
            WORKING_DIRECTORY ${wxWidgets_BUILD_DIR}
        )
        if(NOT wxWidgets_CONFIGURE_RESULT EQUAL 0)
            message(FATAL_ERROR "Failed to configure wxWidgets")
        endif()
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E chdir ${wxWidgets_CONFIG_DIR} make -j4
            RESULT_VARIABLE wxWidgets_COMPILE_RESULT
            WORKING_DIRECTORY ${wxWidgets_BUILD_DIR}
        )
        if(NOT wxWidgets_COMPILE_RESULT EQUAL 0)
            message(FATAL_ERROR "Failed to compile wxWidgets")
        endif()
    else()
        message(STATUS "wxWidgets already configured")
    endif()

endif()

if( NOT IS_WIN )
    find_package( wxWidgets COMPONENTS core base adv html xml xrc aui REQUIRED )

    #if libs are static use --libs --static instead for standard libs.
    set( WX_CONFIG_ARGS_LIBS "--libs --static" )
    set( WXWINDOWS_LIBRARIES "`${CMAKE_WXWINDOWS_WXCONFIG_EXECUTABLE} ${WX_CONFIG_ARGS_LIBS}`" )

    set( CMAKE_WXWINDOWS_CXX_FLAGS "`${CMAKE_WXWINDOWS_WXCONFIG_EXECUTABLE} --cxxflags`" )

    if( wxWidgets_FOUND )
	    include( "${wxWidgets_USE_FILE}" )
	    message( "wxWidgets was found" )
        # cmake_print_variables(wxWidgets_USE_FILE wxWidgets_LIBRARIES)
    else( wxWidgets_FOUND )
	    message( "wxWidgets was not found" )
    endif( wxWidgets_FOUND )

endif()

if( IS_MAC )
file( GLOB SRC_FILES
    src/*.h
    src/*.hpp
    src/*.c
    src/*.mm
    src/*.cpp
    src/*/*.h
    src/*/*.hpp
    src/*/*.c
    src/*/*.cpp
    src/*/*.mm
)
else()
file( GLOB SRC_FILES
    src/*.h
    src/*.hpp
    src/*.c
    src/*.cpp
    src/*/*.h
    src/*/*.hpp
    src/*/*.c
    src/*/*.cpp
)
endif()

if( IS_WIN )
    # Include a RC file for windows
    list( APPEND SRC_FILES icons/icon_win.rc )
endif()
if( IS_MAC )
    # Add an icon for the apple .app file
    list( APPEND SRC_FILES icons/AppIcon.icns )
endif()

if( IS_WIN )
    #-mwindows suppresses CMD
    add_executable( ${PROJECT_NAME} WIN32 ${SRC_FILES} )
    target_link_libraries(${PROJECT_NAME} simpleble)
    target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
elseif( IS_MAC )
    add_executable( ${PROJECT_NAME} MACOSX_BUNDLE ${SRC_FILES} )
    target_link_libraries( ${PROJECT_NAME} ${wxWidgets_LIBRARIES} )
    target_link_libraries(${PROJECT_NAME} simpleble-static)
elseif( IS_LINUX )
    set( CMAKE_CXX_FLAGS "-no-pie" )
    add_executable( ${PROJECT_NAME} ${SRC_FILES} )
    target_link_libraries( ${PROJECT_NAME} ${wxWidgets_LIBRARIES} ${CMAKE_CXX_FLAGS} )
    target_link_libraries(${PROJECT_NAME} simpleble-static)
endif()

#After compilation processing
if( IS_MAC )
    set_target_properties( ${PROJECT_NAME} PROPERTIES
        RESOURCE "icons/AppIcon.icns"
        MACOSX_BUNDLE_ICON_FILE AppIcon.icns
        MACOSX_BUNDLE_COPYRIGHT "Copyright fidoriel 2020"
        MACOSX_BUNDLE_GUI_IDENTIFIER "${PROJECT_NAME}"
        )
    add_custom_command( 
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND plutil -replace NSHighResolutionCapable -bool true ${PROJECT_NAME}.app/Contents/Info.plist
        COMMAND plutil -insert NSBluetoothPeripheralUsageDescription -string "Connect to PoweredUP Hubs" ${PROJECT_NAME}.app/Contents/Info.plist || true
        COMMAND plutil -insert NSBluetoothAlwaysUsageDescription -string "Connect to PoweredUP Hubs" ${PROJECT_NAME}.app/Contents/Info.plist || true
        COMMAND cp -r "../resources/tracks" "${PROJECT_NAME}.app/Contents/Resources"
        COMMAND cp -r "../resources/icons" "${PROJECT_NAME}.app/Contents/Resources"
        )
elseif( CMAKE_SYSTEM_NAME STREQUAL "Linux" AND RELEASE )
    EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )
    message( STATUS "Architecture: ${ARCHITECTURE}" )

    if( "${ARCHITECTURE}" STREQUAL "armv7l" OR "${ARCHITECTURE}" STREQUAL "armv7" )
        set( AppImageRelease "appimagetool-armhf.AppImage" )
    elseif( "${ARCHITECTURE}" STREQUAL "aarch64" OR "${ARCHITECTURE}" STREQUAL "armv8" )
        set( AppImageRelease "appimagetool-aarch64.AppImage" )
    elseif( "${ARCHITECTURE}" STREQUAL "i386" )
        set( AppImageRelease "appimagetool-i686.AppImage" )
    else()
        set( AppImageRelease "appimagetool-x86_64.AppImage" )
    endif()

    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND mkdir -p "AppDir/usr/share/"
        COMMAND mkdir -p "AppDir/usr/share/icons"
        COMMAND cp -r "../resources/tracks/" "AppDir/usr/share/icons/"
        COMMAND cp -r "../resources/icons/" "AppDir/usr/share/icons/"
        COMMAND wget -N https://github.com/AppImage/AppImageKit/releases/download/continuous/${AppImageRelease}
        COMMAND chmod a+x ${AppImageRelease}
        COMMAND cp ../icons/AppIcon.png AppDir/AppIcon.png

        COMMAND rm -f AppDir/${PROJECT_NAME}.desktop
        COMMAND touch AppDir/${PROJECT_NAME}.desktop
        COMMAND echo '[Desktop Entry]' >> AppDir/${PROJECT_NAME}.desktop
        COMMAND echo 'Name=${PROJECT_NAME}' >> AppDir/${PROJECT_NAME}.desktop
        COMMAND echo 'Exec=${PROJECT_NAME}' >> AppDir/${PROJECT_NAME}.desktop
        COMMAND echo 'Icon=AppIcon' >> AppDir/${PROJECT_NAME}.desktop
        COMMAND echo 'Type=Application' >> AppDir/${PROJECT_NAME}.desktop
        COMMAND echo 'Categories=Utility\\;' >> AppDir/${PROJECT_NAME}.desktop

        COMMAND wget -N https://raw.githubusercontent.com/AppImage/AppImageKit/master/src/AppRun.c
        COMMAND gcc AppRun.c -o AppDir/AppRun

        COMMAND ./${AppImageRelease} AppDir
	    )
elseif( WIN32 )
    add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD

        COMMAND if not exist "${PROJECT_NAME}\\Debug\\resources" mkdir "${PROJECT_NAME}\\Debug\\resources"
        COMMAND if not exist "${PROJECT_NAME}\\Debug\\resources\\tracks" mkdir "${PROJECT_NAME}\\Debug\\resources\\tracks"
        COMMAND copy "..\\resources\\tracks" "${PROJECT_NAME}\\Debug\\resources\\tracks"
        COMMAND if not exist "${PROJECT_NAME}\\Debug\\resources\\icons" mkdir "${PROJECT_NAME}\\Debug\\resources\\icons"
        COMMAND copy "..\\resources\\icons" "${PROJECT_NAME}\\Debug\\resources\\icons"

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "simpleble\\Debug\\simpleble.dll"
            "${PROJECT_NAME}\\Debug\\"

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${wxWidgets_BUILD_DIR}lib/vc_x64_dll/wxmsw316ud_core_vc_custom.dll"
            "${PROJECT_NAME}\\Debug\\"

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${wxWidgets_BUILD_DIR}lib/vc_x64_dll/wxbase316ud_vc_custom.dll"
            "${PROJECT_NAME}\\Debug\\"
    )
endif()
