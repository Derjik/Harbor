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

#ifndef HULL_HPP_INCLUDED
#define HULL_HPP_INCLUDED

#include "Visited.hpp"


/*
 * Hull interface (exposes solidity() and some
 * arithmetic operators)
 */

class Hull : public Visited
{
	private:
		unsigned _solidity;	// Solidity (used during collisions)

	public:
		/*** Constructors & Destructors ***/
		Hull(unsigned const s) : _solidity(s) {}
		virtual ~Hull() {};


		/*** Interface to implement ***/

		// Returns the Hulls solidity
		// Note: virtual because the HullComponents use inheritance
		virtual unsigned solidity() const { return _solidity; }

		/*** Arithmetic operators ***/
		virtual bool operator < (Hull const & a) const = 0;
		virtual bool operator <= (Hull const & a) const = 0;

		void accept(Visitor* v)
		{
			v->visit(this);
		}
};

#endif // HULL_HPP_INCLUDED
