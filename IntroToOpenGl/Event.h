#pragma once
#include <functional>
#include <unordered_map>

template<typename T>
class Event
{
	using Handler = std::function<void(T)>;
	std::unordered_map<int, Handler> handlers;
	int nextID = 0;

public:
	int subscribe(Handler h)
	{
		handlers[nextID] = std::move(h);
		return nextID++;
	}

	void unsubscribe(int id) {
		handlers.erase(id);
	}

	void emit(T t)
	{
		for (auto& [id, h] : handlers) h(t);
	}
};