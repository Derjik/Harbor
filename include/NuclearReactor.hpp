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

#ifndef NUCLEARREACTOR_HPP_INCLUDED
#define NUCLEARREACTOR_HPP_INCLUDED

#include "EngineComponent.hpp"	// EngineComponent


/*
 * The Nuclear Reactor is the top Engine booster.
 * It's improvement curve tends to be quadratic.
 */

class NuclearReactor : public EngineComponent
{
	public:
		// Constructor
		NuclearReactor(Engine const * const d) : EngineComponent(d) {}

		/*** Inherited methods ***/
		unsigned speed() const
		{
			unsigned speed = _decoratedEngine->speed();
			// Speed is increased by 50% of its square
			return speed + (speed*speed) / 2;
		}
};

#endif // NUCLEARREACTOR_HPP_INCLUDED
