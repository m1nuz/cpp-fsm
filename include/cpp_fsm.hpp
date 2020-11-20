#pragma once

#include <functional>
#include <map>
#include <variant>

namespace fsm {

    ///
    /// Base state
    ///
    struct base_state {
        virtual ~base_state( ) = default;
        virtual void on_enter( ) {
        }
        virtual void on_leave( ) {
        }
    };
    ///
    /// Typed state
    ///
    template <typename T> struct typed_state : base_state {
        static const std::size_t _type; // State type identifier
    };

    [[maybe_unused]] inline std::size_t generate_state_type( ) noexcept {
        static auto counter = std::size_t{ 0 };
        return counter++;
    }

    template <typename T> const std::size_t typed_state<T>::_type = generate_state_type( );

    ///
    /// Base event
    ///
    struct base_event {
        virtual ~base_event( ) = default;
    };

    ///
    /// Typed event
    ///

    template <typename T> struct typed_event : base_event {
        static const std::size_t _type; // Event type identifier
    };

    [[maybe_unused]] inline std::size_t generate_event_type( ) noexcept {
        static auto counter = std::size_t{ 0 };
        return counter++;
    }

    template <typename T> const std::size_t typed_event<T>::_type = generate_event_type( );

    ///
    /// State Machine
    ///
    template <typename... States> class state_machine {
      public:
        using StateVariant = std::variant<States...>;
        using EventHandler = std::function<void( )>;
        using ActionHandler = std::function<std::optional<StateVariant>( base_state &, const base_event & )>;

        template <typename State, typename Event> void on_action( ActionHandler cb ) {
            _actions[{ State::_type, Event::_type }] = cb;
        }

        template <typename Event> void on_event( EventHandler eh ) {
            _handlers[Event::_type] = eh;
        }

        template <typename Event> void dispatch( Event &&event ) {
            auto new_state = std::visit(
                [&]( auto &s ) -> std::optional<StateVariant> {
                    if ( auto a = _actions.find( { s._type, Event::_type } ); a != _actions.end( ) ) {
                        return a->second( s, std::forward<Event>( event ) );
                    }

                    return std::nullopt;
                },
                _state );

            if ( new_state ) {
                std::visit( []( auto &s ) { s.on_leave( ); }, _state );

                _state = *std::move( new_state );

                std::visit( []( auto &s ) { s.on_enter( ); }, _state );
            }
        }

        template <typename InState, typename Event, typename OutState> void transit_to( ) {
            on_action<InState, Event>( [this]( const auto &, const auto & ) {
                if ( auto h = _handlers.find( Event::_type ); h != _handlers.end( ) ) {
                    h->second( );
                }
                return OutState{ };
            } );
        }

      private:
        StateVariant _state;
        std::map<std::pair<std::size_t, std::size_t>, ActionHandler> _actions;
        std::map<std::size_t, EventHandler> _handlers;
    };

} // namespace fsm
