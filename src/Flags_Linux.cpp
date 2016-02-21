/*
 * Copyright (c) 2015 Julien "Derjik" LAURENT
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

#include "../include/Flags.hpp"

#include <stdexcept>		// std::invalid_argument
#include <iostream>		// std::cout, std::endl

using namespace std;


// Parses the given cycle delay string
void Flags::parseCycleDelay(string const & s)
{
	unsigned delay;

	try
	{
		// Base 10 string to unsigned int conversion
		delay = stoul(s);
	}
	catch(invalid_argument)	// Conversion failure (bad string given)
	{
		cout << "Bad cycle delay value \"" << s;
		cout << "\" (positive or null integer expected)" << endl;

		// Fallback value
		delay = DEFAULT_CYCLE_DELAY;
	}

	_cycleDelay = delay;
}