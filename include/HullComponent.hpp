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

#ifndef HULLCOMPONENT_HPP_INCLUDED
#define HULLCOMPONENT_HPP_INCLUDED

#include "Hull.hpp"	// Hull


/*
 * Hull decorator interface
 */

class HullComponent : public Hull
{
	protected:
		// The decorated Hull
		Hull const * _decoratedHull;

	public:
		/*** Constructors & destructors ***/
		// Note: Hull(0) is a dummy Hull induced by inheritance. The
		// real Hull is the one referenced by _decoratedHull.
		HullComponent(Hull const * const d) : Hull(0), _decoratedHull(d) {}

		virtual ~HullComponent()
		{
			delete _decoratedHull;
		}

		/*** Hull methods reimplementations ***/
		virtual unsigned solidity() const = 0;
		virtual bool operator < (Hull const & a) const
		{
			return (*_decoratedHull) < a;
		}
		virtual bool operator <= (Hull const & a) const
		{
			return (*_decoratedHull) <= a;
		}
};

#endif // HULLCOMPONENT_HPP_INCLUDED
