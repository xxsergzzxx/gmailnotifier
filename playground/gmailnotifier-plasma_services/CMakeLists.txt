project(GmailNotifier-plasmoid)

find_package(KDE4 REQUIRED)

include(KDE4Defaults)

include_directories(${CMAKE_SOURCE_DIR}
                    ${CMAKE_BINARY_DIR}
                    ${KDE4_INCLUDES})

add_subdirectory(src)
