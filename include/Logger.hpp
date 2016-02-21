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

#ifndef LOGGER_HPP_INCLUDED
#define LOGGER_HPP_INCLUDED

#include <fstream>	// std::ofstream
#include <string>	// std::string
#include "Flags.hpp"


/*
 * Basic output stream class made for easy information logging
 * from this project's main classes
 */

class Logger
{
	private:
		// Destination logfile
		std::ofstream _logFile;
		// "New line has begun" flag (speaks for itself)
		bool _newLine;

		// Current logging level
		LogLevel _currentLevel;

	protected:
		/*** Internal methods ***/

		// Get a formatted [HOUR:MIN:SEC] timestamp
		std::string formattedLogTime() const;

		// Get the formatted LogLevel
		std::string level() const;

		// Insert the message prefix
		void prefix()
		{
			// If we're starting a new log line
			if(_newLine)
			{
				// Add the formatted timestamp and a space
				_logFile << formattedLogTime() << " ";
				// Unset the newline flag
				_newLine = false;

				// Add the LogLevel prefix
				_logFile << level() << " ";
			}
		}


	public:
		/*** Constructors & destructors ***/

		Logger(std::string const & pathToLogFile);
		virtual ~Logger();


		/*** Logging methods ***/

		// Out-stream operator (template used for type deduction)
		template <typename T> Logger & operator << (T const & object)
		{
			if(_currentLevel >= Flags::logLevel())
			{
				// Prefix the log line
				prefix();

				// Forward the stream data to the ofstream
				_logFile << object;
			}

			return (*this);
		}

		// Special out-stream operator used along with the custom endl
		Logger & operator << (Logger & (*f) (Logger &));

		// Custom endl has direct access to the Logger's members
		friend Logger & endl(Logger &);

		friend Logger & debug(Logger & l);
		friend Logger & info(Logger & l);
		friend Logger & warn(Logger & l);
		friend Logger & error(Logger & l);
};

// End a message (important!)
Logger & endl(Logger & l);

// Log level setters
Logger & debug(Logger & l);
Logger & info(Logger & l);
Logger & warn(Logger & l);
Logger & error(Logger & l);

#endif // LOGGER_HPP_INCLUDED
