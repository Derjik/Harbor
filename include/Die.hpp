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

#ifndef DIE_HPP_INCLUDED
#define DIE_HPP_INCLUDED

#include <random>	// std::default_random_engine, std::random_device,
			// std::uniform_int_distribution,
			// std::uniform_real_distribution


/*
 * Random Number Generation utility.
 * Just get a die and roll it with integer or floating point min / max values.
 * Auto inits itself at first use.
 */

class Die
{
	private:
		// Random Number Generator (used to feed distributions)
		static std::default_random_engine * _rng;

	public:
		// Init and clean methods (note: Die::clean() MUST be called at
		// the end of the program to avoid memory leaks)
		static void init();
		static void clean();

		// Roll methods (inclusive)
		static int roll(int const min, int const max);
		static float roll(float const min, float const max);
};

#endif // DIE_HPP_INCLUDED
