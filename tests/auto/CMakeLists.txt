# Generated from auto.pro.

if(TARGET Qt::Quick)
    add_subdirectory(qml)
    add_subdirectory(quick)
    add_subdirectory(quicktest)
endif()
add_subdirectory(core)
add_subdirectory(cmake)
add_subdirectory(toolsupport)
if(NOT UIKIT)
    add_subdirectory(qmltest)
endif()
if(TARGET Qt::Quick)
    add_subdirectory(particles)
endif()
if(TARGET Qt::QuickWidgets)
    add_subdirectory(quickwidgets)
endif()
if(TARGET Qt::QmlDomPrivate)
    add_subdirectory(qmldom)
endif()
