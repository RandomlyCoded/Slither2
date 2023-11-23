function(add_test_fixture NAME)
    qt_add_executable(${NAME} ${NAME}.cpp)

    foreach(artifact in ${NAME}.qrc ${NAME}.h ${NAME}.txt)
        if (EXISTS ${artifact})
            target_sources(${NAME} PRIVATE ${artifact})
        endif()
    endforeach()

    target_link_libraries(${NAME} PRIVATE Qt::Test Slither2-Core ${ARGN})
    add_test(NAME ${NAME} COMMAND ${NAME})
endfunction()

add_test_fixture(tst_chunks Qt::Gui)
