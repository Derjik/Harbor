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

#include <ctime>			// time()
#include <cstdlib>			// srand()
#include "../include/Harbor.hpp"	// Harbor
#include "../include/Tower.hpp"		// Tower
#include "../include/Flags.hpp"		// Flags
#include "../include/Die.hpp"		// Die

using namespace std;


int main(int argc, char* argv[])
{
	// Parse arguments
	Flags::parse(argc, argv);

	// If required (see arguments),
	// seed the main RNG
	if(Flags::seedRNGs())
		srand(time(nullptr));

	if(Flags::runCycle())
	{
		// Instantiate the Harbor
		Harbor * h(Harbor::getInstance(25,25));

		// Instantiate the Tower
		Tower t(h);

		// Begin main cycle
		t.cycle(80);

		// Begin out cycle
		t.cycleOut();

		// Clean Harbor instance
		Harbor::deleteInstance();
		// Clean common RNG instance
		Die::clean();
	}
	else
	{
		Flags::printUsage();
	}

	if(Flags::help())
	{
		Flags::printHelp();
	}

	return 0;
}

