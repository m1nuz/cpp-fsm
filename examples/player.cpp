
#include <cpp_fsm.hpp>

#include <iostream>

struct play_event : fsm::typed_event<play_event> {};
struct stop_event : fsm::typed_event<stop_event> {};
struct pause_event : fsm::typed_event<pause_event> {};

struct stopped : fsm::typed_state<stopped> {};
struct playing : fsm::typed_state<playing> {};
struct paused : fsm::typed_state<paused> {};

using Player = fsm::state_machine<stopped, playing, paused>;

Player player;

int main( ) {
    player.transit_to<stopped, play_event, playing>( );
    player.transit_to<playing, stop_event, stopped>( );
    player.transit_to<playing, pause_event, paused>( );
    player.transit_to<paused, play_event, playing>( );
    player.transit_to<paused, stop_event, stopped>( );

    player.on_event<play_event>( []( ) { std::cout << "Handle play" << std::endl; } );
    player.on_event<stop_event>( []( ) { std::cout << "Handle stop" << std::endl; } );
    player.on_event<pause_event>( []( ) { std::cout << "Handle pause" << std::endl; } );

    player.dispatch( play_event{ } );
    player.dispatch( pause_event{ } );
    player.dispatch( play_event{ } );
    player.dispatch( stop_event{ } );

    return 0;
}