# $Id$

project(plasma-dataengine-GmailNotifier)

set(gmailnotifier_dataengine_SRCS
    gmailnotifierengine.cpp
    gmailatomfeedparser.cpp
    gmailnotifiersource.cpp
    gmailnotifierservice.cpp)

kde4_add_plugin(plasma_engine_gmailnotifier
                ${gmailnotifier_dataengine_SRCS})

target_link_libraries(plasma_engine_gmailnotifier
                      ${KDE4_KIO_LIBS}
                      ${KDE4_PLASMA_LIBS})

install(TARGETS plasma_engine_gmailnotifier             DESTINATION ${PLUGIN_INSTALL_DIR}              )
install(FILES   plasma-dataengine-gmailnotifier.desktop DESTINATION ${SERVICES_INSTALL_DIR}            )
install(FILES   gmailnotifierengine.operations          DESTINATION ${DATA_INSTALL_DIR}/plasma/services)
