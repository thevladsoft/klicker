# Project Needs a name ofcourse
project(klicker)
 
# Find the required Libaries
find_package(KDE4 REQUIRED)
include(KDE4Defaults)

 
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   )
 

# We add our source code here
set(klicker_SRCS klickerlauncher.cpp configform.cpp)
kde4_add_ui_files(klicker_SRCS form/configform.ui)

# Now make sure all files get to the right place
kde4_add_plugin(klicker ${klicker_SRCS})



target_link_libraries(klicker 
                      ${KDE4_PLASMA_LIBS} ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} )
 
install(TARGETS klicker
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES plasma-applet-klicker.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})

