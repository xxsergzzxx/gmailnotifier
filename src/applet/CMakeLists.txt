# $Id$

project(plasma-applet-GmailNotifier)

set(gmailnotifier_applet_SRCS
    gmailnotifierapplet.cpp
    gmailnotifierappletconfig.cpp
    gmailnotifierdialog.cpp)

qt4_add_resources(gmailnotifier_applet_SRCS gmailnotifierapplet.qrc)

kde4_add_ui_files(gmailnotifier_applet_SRCS gmailnotifierappletconfig.ui)

kde4_add_plugin(plasma_applet_gmailnotifier
                ${gmailnotifier_applet_SRCS})

target_link_libraries(plasma_applet_gmailnotifier
                      ${KDE4_KIO_LIBS}
                      ${KDE4_PLASMA_LIBS})

install(TARGETS plasma_applet_gmailnotifier         DESTINATION ${PLUGIN_INSTALL_DIR}  )
install(FILES   plasma-applet-gmailnotifier.desktop DESTINATION ${SERVICES_INSTALL_DIR})

# TODO: This should be done using the kde4_install_icons(${ICON_INSTALL_DIR}) command.
#       However, this command does not seem to run when added to the CMakeLists.txt file in the "icons" directory,
#       so I am installing the file manually as a quick fix.
#       -- Mark McCans
#       Don't really know what the correct location for such an icon file would be for now...?!
#       Leaving that path as Mark provided for now.
#       -- GCN
install(FILES   images/gmailnotifier.svgz           DESTINATION ${ICON_INSTALL_DIR}/hicolor/scalable/apps)
kde4_update_iconcache()
