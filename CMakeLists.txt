cmake_minimum_required(VERSION 3.16)

project(KU_INFOBOT VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h

            managers/CourseManager.cpp
            managers/RoutineManager.cpp
            managers/FaqManager.cpp
            managers/AdmissionManager.cpp

            models/Course.cpp
            models/Routine.cpp
            models/Faq.cpp
            models/Admission.cpp

            chatbot/ChatBot.cpp
            chatbot/IntentRecognizer.cpp
            chatbot/ResponseGenerator.cpp
            chatbot/SynonymMapper.cpp
            chatbot/TypoCorrector.cpp

            crawler/WebCrawler.cpp
            crawler/CrawlCache.cpp

            utils/TextMatcher.cpp
            utils/CourseCodeUtils.cpp
            utils/CourseResolver.cpp
            utils/programcodeutils.cpp

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)

    qt_add_executable(KU_INFOBOT
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        utils/programcodeutils.h utils/programcodeutils.cpp
        models/curriculum.h models/curriculum.cpp
    )

else()

    if(ANDROID)

        add_library(KU_INFOBOT SHARED
            ${PROJECT_SOURCES}
        )

    else()

        add_executable(KU_INFOBOT
            ${PROJECT_SOURCES}
        )

    endif()

endif()

target_link_libraries(KU_INFOBOT
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
)

if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.KU_INFOBOT)
endif()

set_target_properties(KU_INFOBOT PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)

install(TARGETS KU_INFOBOT
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(KU_INFOBOT)
endif()