# Generated from quick.pro.

qt_internal_add_example(quick-accessibility)
qt_internal_add_example(animation)
qt_internal_add_example(draganddrop)
qt_internal_add_example(externaldraganddrop)
qt_internal_add_example(canvas)
qt_internal_add_example(imageelements)
qt_internal_add_example(keyinteraction)
qt_internal_add_example(layouts)
add_subdirectory(localstorage)
add_subdirectory(models)
qt_internal_add_example(views)
add_subdirectory(tableview)
qt_internal_add_example(mousearea)
qt_internal_add_example(positioners)
qt_internal_add_example(righttoleft)
add_subdirectory(scenegraph)
qt_internal_add_example(shadereffects)
qt_internal_add_example(text)
qt_internal_add_example(threading)
qt_internal_add_example(touchinteraction)
add_subdirectory(tutorials)
add_subdirectory(customitems)
qt_internal_add_example(imageprovider)
qt_internal_add_example(imageresponseprovider)
qt_internal_add_example(window)
add_subdirectory(particles)
qt_internal_add_example(delegatechooser)
qt_internal_add_example(shapes)
if(QT_FEATURE_opengl OR QT_FEATURE_opengles2 OR QT_FEATURE_opengles3)
    add_subdirectory(rendercontrol)
endif()
if(TARGET Qt::Widgets)
    qt_internal_add_example(embeddedinwidgets)
endif()
if(TARGET Qt::QuickWidgets AND TARGET Qt::Widgets AND (QT_FEATURE_opengl OR QT_FEATURE_opengles2 OR QT_FEATURE_opengles3))
    add_subdirectory(quickwidgets)
endif()
