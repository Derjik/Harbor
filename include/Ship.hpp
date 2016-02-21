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

#ifndef SHIP_HPP_INCLUDED
#define SHIP_HPP_INCLUDED

#include <string>	// std::string
#include <list>		// std::list
#include "Visited.hpp"	// Visited


// Mandatory forward-declarations
class Engine;
class Hull;
class Factory;


/*
 * Common interface for all Ships.
 */

class Ship : public Visited
{
	private:
		// The Ship's name
		std::string _name;

		/*** Ship components ***/
		Engine* _engine;
		Hull* _hull;

	protected:
		// Color to use while displaying on Linux systems
		unsigned _LinuxColor;
		// Color to use while displaying on Windows systems
		unsigned _WindowsColor;

	public:
		/*** Constructor & destructor ***/
		Ship(Factory const * const f, std::string const name="");
		virtual ~Ship();

		/*** Name-related methods ***/
		std::string name() const;
		void setName(std::string const name);

		/*** Engine & Hull related methods ***/
		unsigned speed() const;
		Hull const * hull() const;

		/*** Interface to implement in subclasses ***/
		virtual float failureProbability() const = 0;
		virtual bool accept(unsigned const dockId) const = 0;
		virtual unsigned priority() const = 0;
		virtual std::string type() const = 0;

		void accept(Visitor* v);
		std::list<Visited*> getVisitedItems() const;

		/*** Display-related methods ***/
		void display() const;
};

#endif // SHIP_HPP_INCLUDED
