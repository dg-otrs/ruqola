
set(Ruqola_ddpapi_SRCS
    ddpapi/ddpclient.cpp
    )

set (Ruqola_model_core_srcs    
    model/messagemodel.cpp
    model/roommodel.cpp
    model/roomfilterproxymodel.cpp

    model/usersforroommodel.cpp
    model/usersforroomfilterproxymodel.cpp

    model/usersmodel.cpp

    model/usercompleterfilterproxymodel.cpp
    model/usercompletermodel.cpp

    model/statusmodel.cpp

    model/filesforroommodel.cpp
    model/filesforroomfilterproxymodel.cpp

    model/searchchannelmodel.cpp
    model/searchchannelfilterproxymodel.cpp

    model/inputcompletermodel.cpp

    model/loginmethodmodel.cpp

    model/searchmessagemodel.cpp
    model/searchmessagefilterproxymodel.cpp

    model/rocketchataccountmodel.cpp
    model/rocketchataccountfilterproxymodel.cpp

    model/emoticonfiltermodel.cpp
    model/emoticonmodel.cpp
    model/notificationpreferencemodel.cpp

    model/notificationdesktopdurationpreferencemodel.cpp
    model/notificationdesktopsoundpreferencemodel.cpp

    model/discussionsmodel.cpp
    model/discussionsfilterproxymodel.cpp

    model/emoticoncategoriesmodel.cpp

    model/threadmessagemodel.cpp

    model/listmessagesmodel.cpp
    model/listmessagesmodelfilterproxymodel.cpp

    model/autotranslatelanguagesmodel.cpp

    model/commandsmodel.cpp
    )

set(Ruqola_plugins_srcs
    plugins/pluginauthenticationinterface.cpp
    plugins/pluginauthentication.cpp
    )

set(Ruqola_emoticons_srcs 
    emoticons/emoji.cpp
    emoticons/emojimanager.cpp
    emoticons/unicodeemoticon.cpp
    emoticons/unicodeemoticonparser.cpp
)
    
set(Ruqola_messages_srcs  
    messages/message.cpp
    messages/messageattachment.cpp
    messages/messageurl.cpp
    messages/messagestarred.cpp
    messages/reactions.cpp
    messages/reaction.cpp
    messages/messagepinned.cpp
    messages/messagetranslation.cpp
    )

set(Ruqola_message_convertertextjob_SRCS
    convertertextjob/convertertextabstractjob.cpp
    convertertextjob/translatetextjob.cpp
    )

set(Ruqola_autotranslatelanguage_SRCS
    autotranslatelanguage.cpp
    autotranslatelanguages.cpp
    )

set(Ruqola_command_SRCS
    command.cpp
    commands.cpp
    )

set( Ruqola_kcfg_SRCS settings/ruqolaglobalconfig.kcfgc )

kconfig_add_kcfg_files(Ruqola_kcfg_SRCS ${Ruqola_kcfg_SRCS})


set (Ruqola_core_srcs
    ${Ruqola_kcfg_SRCS}
    ${Ruqola_command_SRCS}
    ${Ruqola_autotranslatelanguage_SRCS}
    ${Ruqola_message_convertertextjob_SRCS}
    ${Ruqola_emoticons_srcs}
    ${Ruqola_messages_srcs}
    ${Ruqola_model_core_srcs}
    ${Ruqola_plugins_srcs}
    ${Ruqola_ddpapi_SRCS}
    authenticationinfo.cpp
    room.cpp
    roomwrapper.cpp
    notificationoptionswrapper.cpp
    ruqola.cpp
    ruqolautils.cpp
    rocketchatbackend.cpp
    messagequeue.cpp
    rocketchatmessage.cpp
    typingnotification.cpp
    changetemporarystatus.cpp
    user.cpp
    utils.cpp
    clipboardproxy.cpp
    otr.cpp
    otrmanager.cpp
    abstractwebsocket.cpp
    ruqolawebsocket.cpp
    rocketchataccount.cpp
    rocketchataccountsettings.cpp
    ruqolalogger.cpp
    ruqolaregisterengine.cpp
    ruqolaserverconfig.cpp
    rocketchatcache.cpp
    texthighlighter.cpp
    textconverter.cpp
    loadrecenthistorymanager.cpp
    file.cpp
    channel.cpp
    inputtextmanager.cpp
    authenticationmanager.cpp

    accountmanager.cpp
    managerdatapaths.cpp
    messagecache.cpp

    notificationoptions.cpp

    syntaxhighlightingmanager.cpp

    receivetypingnotificationmanager.cpp

    serverconfiginfo.cpp
    notificationpreferences.cpp

    roles.cpp
    role.cpp

    avatarmanager.cpp

    discussion.cpp
    discussions.cpp

    emoticoncategory.cpp

    listmessages.cpp

    messagedownloadmanager.cpp

    fileattachments.cpp
    commands.cpp

    accountroomsettings.cpp
)

if (NOT WIN32)
    set(Ruqola_core_srcs ${Ruqola_core_srcs} unityservicemanager.cpp)
endif()

if (NOT ANDROID)
    list(APPEND Ruqola_core_srcs notification.cpp)
endif()

ecm_qt_declare_logging_category(Ruqola_core_srcs HEADER ruqola_debug.h IDENTIFIER RUQOLA_LOG CATEGORY_NAME org.kde.ruqola)
ecm_qt_declare_logging_category(Ruqola_core_srcs HEADER ruqola_message_debug.h IDENTIFIER RUQOLA_MESSAGE_LOG CATEGORY_NAME org.kde.ruqola.message)
ecm_qt_declare_logging_category(Ruqola_core_srcs HEADER ruqola_ddpapi_debug.h IDENTIFIER RUQOLA_DDPAPI_LOG CATEGORY_NAME org.kde.ruqola.ddpapi)
ecm_qt_declare_logging_category(Ruqola_core_srcs HEADER ruqola_ddpapi_command_debug.h IDENTIFIER RUQOLA_DDPAPI_COMMAND_LOG CATEGORY_NAME org.kde.ruqola.ddpapi.command)
ecm_qt_declare_logging_category(Ruqola_core_srcs HEADER ruqola_unknown_collectiontype_debug.h IDENTIFIER RUQOLA_UNKNOWN_COLLECTIONTYPE_LOG CATEGORY_NAME org.kde.ruqola.ddp.collectiontype)

qt5_add_resources(libruqolacore_RSC ruqolacore.qrc)

add_library(libruqolacore ${Ruqola_core_srcs} ${libruqolacore_RSC})

generate_export_header(libruqolacore BASE_NAME libruqolacore)



target_link_libraries(libruqolacore
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Qml
    Qt5::Quick
    Qt5::WebSockets
    Qt5::Network
    Qt5::NetworkAuth
    KF5::CoreAddons
    KF5::I18n
    KF5::Notifications
    KF5::SyntaxHighlighting
    librocketchatrestapi-qt5
    KF5::ConfigGui
    KF5::ConfigWidgets
)

if (NOT WIN32)
    target_link_libraries(libruqolacore Qt5::DBus)
endif()

if (WIN32 OR APPLE)
    target_link_libraries(libruqolacore KF5::IconThemes)
endif()

if (Qt5Keychain_FOUND)
    target_link_libraries(libruqolacore qt5keychain)
    target_include_directories(libruqolacore PRIVATE ${QTKEYCHAIN_INCLUDE_DIRS})
endif()

set_target_properties(libruqolacore
    PROPERTIES OUTPUT_NAME ruqolacore VERSION ${RUQOLA_LIB_VERSION} SOVERSION ${RUQOLA_LIB_SOVERSION}
    )

if (BUILD_TESTING)
    add_subdirectory(autotests)
endif()

install(TARGETS libruqolacore ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)

if (ANDROID)
    install(FILES ruqolacore-android-dependencies.xml DESTINATION ${KDE_INSTALL_LIBDIR})
endif()
