#pragma once
#pragma once
#include <iostream>
#include <windows.h>
// remember to undef all the dreadful #defines in windows.h one by one as they cause problems.

namespace LE
{
	class Timer
	{
		LARGE_INTEGER timer;
		float inverseTimerFreq;
		LARGE_INTEGER TimeNow()
		{
			LARGE_INTEGER val;
			QueryPerformanceCounter(&val);
			return val;
		}
	public:
		Timer()
		{
			LARGE_INTEGER val;
			QueryPerformanceFrequency(&val);
			inverseTimerFreq = 1.0f / (float)val.QuadPart;
		}
		void Reset()
		{
			timer = TimeNow();
		}
		float Elapsed()
		{
			long long int elapsed = TimeNow().QuadPart - timer.QuadPart;
			return (float)elapsed * inverseTimerFreq;
		}
	};
}