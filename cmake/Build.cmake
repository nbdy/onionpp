include(cmake/target/onionpp.cmake)

if (TORPP)
    include(cmake/target/torpp.cmake)
endif ()

if (PYTHON_BINDINGS)
    include(cmake/target/binding/python.cmake)
endif ()
