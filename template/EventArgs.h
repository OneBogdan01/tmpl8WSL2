#pragma once

#include "Delegate.h"
#include "glm/glm.hpp"

// Base class for all event args.
class EventArgs
{
public:
	EventArgs() = default;
	virtual ~EventArgs() = default;
};

// Define an event that takes a reference to EventArgs
// as it's only argument.
using Event = Delegate<EventArgs&>;
using VoidEvent = sig::signal<void()>;
using VoidEventUInt = sig::signal<void(size_t)>;
using VoidEventVec3 = sig::signal<void(glm::vec3*)>;
