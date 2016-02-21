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

#include "../include/PrestigiousManufactory.hpp"

#include "../include/Die.hpp"			// Die
#include "../include/ExpensiveEngine.hpp"	// ExpensiveEngine
#include "../include/ExpensiveHull.hpp"		// ExpensiveHull
#include "../include/Turbocharger.hpp"		// Turbocharger
#include "../include/NuclearReactor.hpp"	// NuclearReactor
#include "../include/GoldPlating.hpp"		// GoldPlating
#include "../include/TitaniumPlating.hpp"	// TitaniumPlating

// Return a concrete Engine referenced by a generic pointer
Engine* PrestigiousManufactory::createEngine() const
{
	Engine* e(nullptr);
	unsigned result(Die::roll(1, 6));

	switch(result)
	{
		case 6:
			e = new NuclearReactor(new ExpensiveEngine());
		break;

		case 5:
		case 4:
			e = new Turbocharger(new ExpensiveEngine());
		break;

		default:
			e = new ExpensiveEngine();
		break;
	}
	return e;
}

// Return a concrete Hull referenced by a generic pointer
Hull* PrestigiousManufactory::createHull() const
{
	Hull* h(nullptr);
	unsigned result(Die::roll(1, 6));

	switch(result)
	{
		case 6:
			h = new GoldPlating(
				new TitaniumPlating(
					new ExpensiveHull()));
		break;

		case 5:
			h = new TitaniumPlating(new ExpensiveHull());
		break;

		case 4:
			h = new GoldPlating(new ExpensiveHull());
		break;

		default:
			h = new ExpensiveHull();
		break;
	}
	return h;
}
