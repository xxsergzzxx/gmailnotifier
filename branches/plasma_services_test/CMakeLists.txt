project(plasma-dataengine-pst)  # pst stands for plasma_services_test

find_package(KDE4 REQUIRED)

include(KDE4Defaults)

include_directories(${CMAKE_SOURCE_DIR}
                    ${CMAKE_BINARY_DIR}
                    ${KDE4_INCLUDES})

set(pst_dataengine_SRCS
    pstengine.cpp
    pstsource.cpp
    pstservice.cpp
    pstjob.cpp)

kde4_add_plugin(plasma_engine_pst
                ${pst_dataengine_SRCS})

target_link_libraries(plasma_engine_pst
                      ${KDE4_KIO_LIBS}
                      ${KDE4_PLASMA_LIBS})

install(TARGETS plasma_engine_pst             DESTINATION ${PLUGIN_INSTALL_DIR}              )
install(FILES   plasma-dataengine-pst.desktop DESTINATION ${SERVICES_INSTALL_DIR}            )
install(FILES   pst.operations                DESTINATION ${DATA_INSTALL_DIR}/plasma/services)
