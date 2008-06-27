project(GmailNotifier-plasmoid)

find_package(KDE4 REQUIRED)
find_package(Plasma REQUIRED)

include(KDE4Defaults)

include_directories(${CMAKE_SOURCE_DIR}
                    ${CMAKE_BINARY_DIR}
                    ${KDE4_INCLUDES}
                    ${PLASMA_INCLUDE_DIR})

add_subdirectory(src)
