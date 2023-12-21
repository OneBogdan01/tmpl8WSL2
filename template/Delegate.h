#pragma once
//from https://github.com/jpvanoosten/signals
#include "signals.hpp"
#include <iostream>

// Delegate that holds function callbacks.
// For simplicity, delegates are limited to void return types.
template <typename... Args>
class Delegate
{
public:
	using signal = sig::signal<void(Args...)>;
	using connection = sig::connection;

	// Adds a function callback to the delegate.
	template <typename Func>
	connection operator+=(Func&& f)
	{
		return m_Callbacks.connect(std::forward<Func>(f));
	}

	// Remove a function callback.
	// Returns the number of functions removed.
	template <typename Func>
	std::size_t operator-=(Func&& f)
	{
		return m_Callbacks.disconnect(std::forward<Func>(f));
	}

	// Invoke the delegate.
	// All connected callbacks are invoked.
	void operator()(Args... args)
	{
		m_Callbacks(std::forward<Args>(args)...);
	}

private:
	signal m_Callbacks;
};
