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

#include "../include/Flags.hpp"

#include <vector>		// std::vector
#include <iostream>		// std::cout, std::endl

using namespace std;

unsigned int Flags::_cycleDelay = DEFAULT_CYCLE_DELAY;
bool Flags::_seedRNGs = true;
bool Flags::_randomizeDocks = true;
bool Flags::_runCycle = false;
bool Flags::_help = false;
LogLevel Flags::_logLevel = INFO;


/*
 * Parses the given argument vector, looking for known flags.
 * Uses sub-parsers in the most "complex" cases (e.g. integer values)
 */
void Flags::parse(int const argc, char const * const argv[])
{
	// Convert the C-style arguments array into a STL vector
	vector<string> args;
	for(int i = 0 ; i < argc ; ++i)
		args.push_back(argv[i]);

	// Hint: ignore program basename
	// For each argument, try parsing
	for(unsigned i = 1 ; i < args.size() ; ++i)
	{
		if(args[i] == "--delay" || args[i] == "-d")
			if(i+1 < args.size())
				parseCycleDelay(args[i+1]);

		if(args[i] == "-v" || args[i] == "--verbosity")
			if(i+1 < args.size())
				parseLogLevel(args[i+1]);

		if(args[i] == "--ordered-docks" || args[i] == "-o")
			_randomizeDocks = false;

		if(args[i] == "--no-seed" || args[i] == "-n")
			_seedRNGs = false;

		if(args[i] == "--help" || args[i] == "-h")
			_help = true;

		if(args[i] == "run")
			_runCycle = true;
	}
}

// Parse a log level
void Flags::parseLogLevel(string const & s)
{
	LogLevel l = INFO;

	if(s == "DEBUG")
		l = DEBUG;
	else if(s == "INFO")
		l = INFO;
	else if(s == "WARN")
		l = WARN;
	else if(s == "ERROR")
		l = ERROR;

	_logLevel = l;
}

// Print the help message
void Flags::printHelp()
{
	cout << endl;
	cout << "\t-h --help" << endl;
	cout << "\t\tPrint the help message." << endl << endl;

	cout << "\t-o --ordered-docks" << endl;
	cout << "\t\tDon't mix dock IDs accross the Harbor's sides." << endl;
	cout << "\t\t(keep them clockwise-ordered)" << endl << endl;

	cout << "\t-n --no-seed" << endl;
	cout << "\t\tDon't seed the Random Number Generators. (Dice" << endl;
	cout << "\t\trolls obtained via Die::roll(...) will stay the" << endl;
	cout << "\t\tsame)" << endl << endl;

	cout << "\t-d --delay <unsigned integer>" << endl;
	cout << "\t\tSet the delay (in milliseconds) between two" << endl;
	cout << "\t\tTower cycles" << endl << endl;

	cout << "\t-v --verbosity <DEBUG|INFO|WARN|ERROR>" << endl;
	cout << "\t\tSet the logging verbosity" << endl << endl;

	cout << "\trun" << endl;
	cout << "\t\tRun the simulation (nothing runs if not set)" << endl;
}

// Print the usage message
void Flags::printUsage()
{
	cout << "Usage: bin/harbor_<OS> [options...] [run]" << endl;
	cout << "(see -h or --help for available options)" << endl;
}
