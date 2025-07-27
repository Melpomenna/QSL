function(QSLInstallProject)

    install(TARGETS Memory RUNTIME DESTINATION lib COMPONENT QSL)

    install(DIRECTORY ${QSL_INCLUDE_DIR}/QSL DESTINATION include COMPONENT QSL)

    set(CPACK_PACKAGE_NAME "QSL")
    include(CPack)
endfunction(QSLInstallProject)
