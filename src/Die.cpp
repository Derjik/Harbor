/*
 * Copyright (c) 2016 Julien "Derjik" Laurent <julien.laurent@engineer.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "../include/Die.hpp"

#include "../include/Flags.hpp"	// Flags

#ifdef _WIN32			// Windows will need this to seed the RNG
#include <ctime>		// time()
#endif

using namespace std;


default_random_engine * Die::_rng(nullptr);

// Initialize the RNG engine
void Die::init()
{
	// Prevent overwriting an already initialized RNG
	// (and consequent memory leak)
	if(_rng == nullptr)
	{
		if(Flags::seedRNGs())
		{
			// Get a "genuine random" source
#ifndef _WIN32	// Win32 doesn't handle "random_device"
			random_device rd;
			// Seed the PRNG with it
			_rng = new default_random_engine(rd());
#else
			_rng = new default_random_engine(time(nullptr));
#endif
		}
		else	// Don't seed the PRNG (debug purposes)
			_rng = new default_random_engine();
	}
}

// Clean the RNG engine
void Die::clean()
{
	// Wipe the carpet
	delete _rng;
	_rng = nullptr;
}

// Get a random int
int Die::roll(int const min, int const max)
{
	// Auto init
	if(_rng == nullptr)
		init();

	uniform_int_distribution<int> d(min, max);
	return d(*_rng);
}

// Get a random float
float Die::roll(float const min, float const max)
{
	// Auto init
	if(_rng == nullptr)
		init();

	uniform_real_distribution<float> d(min, max);
	return d(*_rng);
}
