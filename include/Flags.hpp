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

#ifndef FLAGS_HPP_INCLUDED
#define FLAGS_HPP_INCLUDED

#include <string>	// std::string

#define DEFAULT_CYCLE_DELAY 150

// Logging level
enum LogLevel
{
	DEBUG	=0,
	INFO	=1,
	WARN	=2,
	ERROR	=3
};


/*
 * Execution flags, set by command line:
 *
 *	-h --help
 *		Print the help message.
 *
 *	-o --ordered-docks
 *		Don't mix dock IDs accross the Harbor's sides.
 *		(keep them clockwise-ordered)
 *
 *	-n --no-seed
 *		Don't seed the Random Number Generators. (Dice
 *		rolls obtained via Die::roll(...) will stay the
 *		same)
 *
 *	-d --delay <unsigned integer>
 *		Set the delay (in milliseconds) between two
 *		Tower cycles
 *
 *	-v --verbosity <DEBUG|INFO|WARN|ERROR>
 *		Set the logging verbosity
 */

class Flags
{
	private:
		/*** Flags ***/
		// Delay between two Tower::cycle() frames in ms
		static unsigned _cycleDelay;
		// Indicates wether RNGs should be seeded or not
		static bool _seedRNGs;
		// Indicates wether docks should be randomized or not
		static bool _randomizeDocks;
		// Indicates wether a cycle should be started
		static bool _runCycle;
		// Indicates wether the help message should be started
		static bool _help;
		// Sets the minimum verbosity level to be displayed
		static LogLevel _logLevel;

		/*** Sub-parsers ***/
		static void parseCycleDelay(std::string const &);
		static void parseLogLevel(std::string const &);

	public:
		// Main arguments parser
		static void parse(int const argc, char const * const argv[]);

		/*** Messages printers ***/
		static void printHelp();
		static void printUsage();

		/*** Trivial getters ***/
		static unsigned cycleDelay()
		{
			return _cycleDelay;
		}
		static bool seedRNGs()
		{
			return _seedRNGs;
		}
		static bool randomizeDocks()
		{
			return _randomizeDocks;
		}
		static bool runCycle()
		{
			return _runCycle;
		}
		static bool help()
		{
			return _help;
		}
		static LogLevel logLevel()
		{
			return _logLevel;
		}
};

#endif // FLAGS_HPP_INCLUDED
