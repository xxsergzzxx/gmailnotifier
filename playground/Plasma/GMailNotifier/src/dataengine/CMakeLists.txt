project(plasma-dataengine-GMailNotifier)

set(gmailnotifier_dataengine_SRCS
    gmailnotifierengine.cpp
    gmailatomfeedparser.cpp)

kde4_add_plugin(plasma_engine_gmailnotifier
                ${gmailnotifier_dataengine_SRCS})

target_link_libraries(plasma_engine_gmailnotifier
                      ${KDE4_KIO_LIBS}
                      ${PLASMA_LIBS})

install(TARGETS plasma_engine_gmailnotifier             DESTINATION ${PLUGIN_INSTALL_DIR}  )
install(FILES   plasma-dataengine-gmailnotifier.desktop DESTINATION ${SERVICES_INSTALL_DIR})
