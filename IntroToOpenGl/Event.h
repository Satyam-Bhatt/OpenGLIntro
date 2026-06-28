#pragma once
#include <functional>
#include <unordered_map>

// Genrally we use Typename T but if we want to pass in multiple arguments then we need these 3 dots(...) and then we can pass multiple arguments. Args is just a name and can be anything.
// One work around if we don't want to have 3 dots is to pass a struct but that just means writing a new struct everytime.

template<typename... Args>
class Event
{
    // Handler is now a callable that accepts whatever Args... expands to.
    //
    // Event<int, int>  → std::function<void(int, int)>
    // Event<float>     → std::function<void(float)>
    // Event<>          → std::function<void()>
    using Handler = std::function<void(Args...)>;

    // Maps a unique int token → the stored callable.
    // The token is what lets callers unsubscribe a specific handler later.
    std::unordered_map<int, Handler> handlers;

    int nextID = 0;

public:
    // subscribe() accepts any callable matching void(Args...) and stores it.
    // Returns a token the caller holds onto for unsubscribing.
    //
    // LAMBDA RECAP:
    //   A lambda is an anonymous function you write inline.
    //   [&](int w, int h) { ... }
    //    ^   ^              ^
    //    |   arguments      body
    //    capture list: [&] = capture everything from outer scope by reference
    //                  [=] = capture by value (copy)
    //                  []  = capture nothing
    //
    // The compiler turns a lambda into a small hidden struct with operator().
    // std::function<void(Args...)> wraps that struct so we can store it.
    // std::move(h) transfers it into the map without copying.
    //
    // EXAMPLES:
    //
    //   Event<int, int> onWindowResize;
    //   int token = onWindowResize.subscribe([&](int w, int h) {
    //       idBuffer.resize(w, h);
    //       camera.setAspect((float)w / h);
    //   });
    //
    //   Event<float> onUpdate;
    //   int token = onUpdate.subscribe([&](float dt) {
    //       player.update(dt);
    //   });
    //
    //   Event<> onShutdown;
    //   int token = onShutdown.subscribe([&]() {
    //       saveGameState();
    //   });
    int subscribe(Handler h)
    {
        handlers[nextID] = std::move(h);
        return nextID++;
    }

    // unsubscribe() removes the handler with the given token.
    // After this, emit() will skip that handler entirely.
    //
    // EXAMPLE:
    //   // A loading screen subscribes to resize during loading only
    //   int token = onWindowResize.subscribe([&](int w, int h) {
    //       loadingScreen.reflow(w, h);
    //   });
    //
    //   // Once loading is done, we no longer need to reflow it
    //   onWindowResize.unsubscribe(token);
    void unsubscribe(int id) {
        handlers.erase(id);
    }

    // emit() fires the event — calls every stored handler with the given args.
    //
    // Args&&... and std::forward are used here for perfect forwarding:
    // this passes arguments exactly as they came in (preserving move semantics)
    // rather than always copying them. Matters if you ever fire an event
    // passing a large struct or a unique_ptr.
    //
    // auto& [id, h] is C++17 structured bindings —
    // unpacks each map entry into id (the token) and h (the handler)
    // so you don't have to write entry.first / entry.second.
    //
    // EXAMPLES:
    //   onWindowResize.emit(1920, 1080);
    //   onUpdate.emit(0.016f);
    //   onShutdown.emit();
    void emit(Args... args)
    {
        for (auto& [id, h] : handlers) h(args...);
    }
};