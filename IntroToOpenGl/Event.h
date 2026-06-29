#pragma once
#include <functional>
#include <unordered_map>

// Genrally we use Typename T but if we want to pass in multiple arguments then we need these 3 dots(...) and then we can pass multiple arguments. Args is just a name and can be anything.
// One work around if we don't want to have 3 dots is to pass a struct but that just means writing a new struct everytime.

template<typename... Args>
class Event
{
    // Handler is a short form of writing std::function<void(Args...)> so that we don't have to write this big line repeatedly
    using Handler = std::function<void(Args...)>;

    // Maps a unique int token → the stored callable.
    // The token is what lets callers unsubscribe a specific handler later.
    std::unordered_map<int, Handler> handlers;

    int nextID = 0;

public:
    // Subscribe takes in a lambda function of similar type as the handler. So basically it is a function calling another function. It returns an int which is useful for unsubscribing the function
    // Lamda function is an anonymous function you don't need to define seperately
    //   [&](int w, int h) { ... }
    //    ^   ^              ^
    //    |   arguments      body
    //    capture list: [&] = capture everything from outer scope by reference
    //                  [=] = capture by value (copy)
    //                  []  = capture nothing
    // EXAMPLE:
    //
    //  Event<int, int> onWindowResize;
    //  int token = onWindowResize.subscribe(
    //      [&](int w, int h) <-- This creates a funciton that takes in 2 ints passed by value and passes it to two functions(resize and setAspect) sequentially calling them. Both the ints are received from emit function
    //                        <-- [&] ensures that idBuffer and camera (2 objects) are passed by reference 
    //      {
    //         idBuffer.resize(w, h); <-- Calls this function
    //         camera.setAspect((float)w / h); <-- and calls this function
    //      });
    // 
    //  UNDER THE HOOD? -  The compiler transforms a lambda into a hidden struct behind the scenes like defined below
    //  struct __Lambda_xyz
    //  {
    //      // [&] — captured variables stored as references inside the struct
    //      IDBuffer& idBuffer;
    //      Camera& camera;

    //      // (int w, int h) — the parameters become arguments to operator()
    //      void operator()(int w, int h) const
    //      {
    //          idBuffer.resize(w, h);
    //          camera.setAspect((float)w / h);
    //      }
    //  };
    // 
    // ====================== MORE EXAMPLES ================================
    //
    //  Event<float> onUpdate;
    //  int token = onUpdate.subscribe([&](float dt) {
    //      player.update(dt);
    //  });
    //
    //  Event<> onShutdown;
    //  int token = onShutdown.subscribe([&]() {
    //      saveGameState();
    //  });
    int subscribe(Handler h)
    {
        // Move the h into the map without copying it
        handlers[nextID] = std::move(h);
        return nextID++;
    }

    // Unsubscribe just removes the function from the hashmap
    // token that is provided is used for this
    void unsubscribe(int id) {
        handlers.erase(id);
    }

    // Triggers all the functions that are stored in the hashmap by looping over each token and calling the relevant function
    void emit(Args... args)
    {
        for (auto& [id, h] : handlers) h(args...);
    }
};