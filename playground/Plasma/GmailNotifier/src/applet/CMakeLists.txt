project(plasma-applet-GmailNotifier)

set(gmailnotifier_applet_SRCS
    gmailnotifierapplet.cpp
    gmailnotifierappletconfig.cpp)

qt4_add_resources(gmailnotifier_applet_SRCS gmailnotifierapplet.qrc)

kde4_add_ui_files(gmailnotifier_applet_SRCS gmailnotifierappletconfig.ui)

kde4_add_plugin(plasma_applet_gmailnotifier
                ${gmailnotifier_applet_SRCS})

target_link_libraries(plasma_applet_gmailnotifier
                      ${KDE4_KIO_LIBS}
                      ${PLASMA_LIBS})

install(TARGETS plasma_applet_gmailnotifier         DESTINATION ${PLUGIN_INSTALL_DIR}  )
install(FILES   plasma-applet-gmailnotifier.desktop DESTINATION ${SERVICES_INSTALL_DIR})
