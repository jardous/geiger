// TinyTimer.h - Tiny timer library for Arduino
// Copyright 2016-2017 Alexandre Bouvier <abouvier@student.42.fr>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef TINYTIMER_H
#define TINYTIMER_H

#include <Arduino.h>

template <unsigned long (*now)() = millis>
class TinyTimer
{
public:
	typedef decltype(now()) time_t;
	typedef void (*callback_t)();

private:
	callback_t m_callback;
	time_t m_delay;
	size_t m_repeat;
	time_t m_now;

public:
	TinyTimer()
	{
		init(nullptr);
	}

	TinyTimer(callback_t callback)
	{
		init(callback, 0);
	}

	TinyTimer(callback_t callback, time_t delay)
	{
		init(callback, delay, 0);
	}

	TinyTimer(callback_t callback, time_t delay, size_t repeat)
	{
		init(callback, delay, repeat);
	}

	void init(callback_t callback)
	{
		init(callback, 0);
	}

	void init(callback_t callback, time_t delay)
	{
		init(callback, delay, 0);
	}

	void init(callback_t callback, time_t delay, size_t repeat)
	{
		m_callback = callback;
		m_delay = delay;
		m_repeat = repeat;
		restart();
	}

	template <size_t size>
	static void update(TinyTimer (&timers)[size])
	{
		for (auto &&timer : timers)
			timer.update();
	}

	void update()
	{
		if (m_callback)
		{
			if (now() - m_now >= m_delay)
			{
				m_callback();
				if (m_repeat && !--m_repeat)
					disable();
				m_now += m_delay;
			}
		}
	}

	void restart()
	{
		m_now = now();
	}

	void disable()
	{
		m_callback = nullptr;
	}
};

#endif
