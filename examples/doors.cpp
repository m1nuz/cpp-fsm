#include <cpp_fsm.hpp>

#include <iostream>

struct OpenEvent : fsm::typed_event<OpenEvent> {};

struct CloseEvent : fsm::typed_event<CloseEvent> {};

struct LockEvent : fsm::typed_event<LockEvent> {};

struct UnlockEvent : fsm::typed_event<UnlockEvent> {};

struct ClosedState : fsm::typed_state<ClosedState> {};

struct OpenState : fsm::typed_state<OpenState> {};

struct LockedState : fsm::typed_state<LockedState> {};

using Door = fsm::state_machine<OpenState, LockedState, ClosedState>;

Door door;

int main( ) {

    door.transit_to<OpenState, CloseEvent, ClosedState>( );
    door.transit_to<ClosedState, OpenEvent, OpenState>( );
    door.transit_to<ClosedState, LockEvent, LockedState>( );
    door.transit_to<LockedState, UnlockEvent, ClosedState>( );

    door.on_event<OpenEvent>( []( ) { std::cout << "Handle open" << std::endl; } );
    door.on_event<CloseEvent>( []( ) { std::cout << "Handle close" << std::endl; } );
    door.on_event<LockEvent>( []( ) { std::cout << "Handle lock" << std::endl; } );
    door.on_event<UnlockEvent>( []( ) { std::cout << "Handle unlock" << std::endl; } );

    door.dispatch( CloseEvent{ } );
    door.dispatch( LockEvent{ } );
    door.dispatch( CloseEvent{ } );
    door.dispatch( UnlockEvent{ } );
    door.dispatch( OpenEvent{ } );
    door.dispatch( OpenEvent{ } );

    return 0;
}