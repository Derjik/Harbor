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

#include "../include/Logger.hpp"

#include <iomanip>	// std::setw, std::setfill...
#include <sstream>	// std::ostringstream

#define DEFAULT_LOGLEVEL INFO

using namespace std;


Logger::Logger(string const & pathToLogFile) : _newLine(true)
{
	// Open the given log file in trucate + write mode
	_logFile.open(pathToLogFile, ios::trunc | ios::out);
}

Logger::~Logger()
{
	// Close it properly
	_logFile.close();
}

string Logger::formattedLogTime() const
{
	// Get a raw time value
	time_t rawtime(time(nullptr));
	// Convert it into a "tm" structure
	tm const * timeinfo(localtime(&rawtime));
	// Prepare the output string stream
	ostringstream s;

	// Insert data
	s << "[" << setw(2) << setfill('0') << timeinfo->tm_hour;
	s << ":" << setw(2) << setfill('0') << timeinfo->tm_min;
	s << ":" << setw(2) << setfill('0') << timeinfo->tm_sec;
	s << "]";

	// Stringify!
	return s.str();
}

string Logger::level() const
{
	string l("");

	switch(_currentLevel)
	{
		case DEBUG:
			l = "[DEBUG]";
		break;

		case INFO:
			l = "[INFO]";
		break;

		case WARN:
			l = "[WARNING]";
		break;

		case ERROR:
			l = "[ERROR]";
		break;
	}

	return l;
}

Logger & Logger::operator << (Logger & (*f) (Logger &))
{
	// Apply f() to the current Logger
	return f(*this);
	// Note: this construct is strongly inspired from
	// the std::basic_ostream & std::endl approach,
	// giving transparency to the consequent calls
}

// This custom endl ends the current "log line" in the Logger
// and sets the logging level back to default
Logger & endl(Logger & l)
{
	// Add the newline character
	l << "\n";
	// Set the newline flag
	l._newLine = true;
	// Set the level back to INFO
	l._currentLevel = DEFAULT_LOGLEVEL;

	return l;
}

Logger & debug(Logger & l)
{
	l._currentLevel = DEBUG;
	
	return l;
}

Logger & info(Logger & l)
{
	l._currentLevel = INFO;
	
	return l;
}

Logger & warn(Logger & l)
{
	l._currentLevel = WARN;
	
	return l;
}

Logger & error(Logger & l)
{
	l._currentLevel = ERROR;
	
	return l;
}
