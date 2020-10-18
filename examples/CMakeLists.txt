find_package(cpp-fsm REQUIRED)

add_executable(doors-example doors.cpp)

target_compile_features(doors-example
    INTERFACE
        cxx_std_17
)

target_link_libraries(doors-example 
    PUBLIC
        cpp-fsm::cpp-fsm
)

add_executable(player-example player.cpp)

target_compile_features(player-example
    INTERFACE
        cxx_std_17
)

target_link_libraries(player-example 
    PUBLIC
        cpp-fsm::cpp-fsm
)
