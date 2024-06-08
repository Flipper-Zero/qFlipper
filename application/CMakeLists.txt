qt_add_executable(${CMAKE_PROJECT_NAME})

target_sources(${CMAKE_PROJECT_NAME}
    PRIVATE
        "main.cpp"
        "application.cpp"
        "applicationupdater.cpp"
        "applicationupdateregistry.cpp"
        "screencanvas.cpp"
        "systemfiledialog.cpp"
        "qtsingleapplication/qtlocalpeer.cpp"
        "qtsingleapplication/qtsingleapplication.cpp"
        "qtsingleapplication/qtsinglecoreapplication.cpp"
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE
        dfu
        backend
        Qt::Gui
        Qt::Qml
        Qt::Quick
        Qt::QuickControls2
        Qt::Widgets
)
