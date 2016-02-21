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

#ifndef MILITARYSHIP_HPP_INCLUDED
#define MILITARYSHIP_HPP_INCLUDED

#include "Ship.hpp"	// Ship


/*
 * The Military Ship: highest priority, only
 * accepts >= 20 dock IDs, and has a 50% engine
 * failure rate.
 */

class MilitaryShip : public Ship
{
	public:
		// Constructor
		MilitaryShip(Factory const * const f, std::string s = "");

		/*** Inherited methods ***/
		float failureProbability() const;
		bool accept(unsigned const dockId) const;
		unsigned priority() const;
		std::string type() const
		{
			return "Military Ship";
		}
};

#endif // MILITARYSHIP_HPP_INCLUDED
