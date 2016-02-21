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

#ifndef TOWER_HPP_INCLUDED
#define TOWER_HPP_INCLUDED

#include <list>			// std::list
#include <vector>		// std::vector

#include "Point.hpp"		// Point
#include "Logger.hpp"		// Logger, custom endl
#include "XMLVisitor.hpp"	// XMLVisitor


// Mandatory forward-declarations
class Harbor;
class Ship;


/*
 * Manages Ships on a given Harbor instance, applying several
 * algorithms to progressively fill its surface with Ships
 * and lead them to their reserved docks.
 */

class Tower
{
	private:
		// Logging system
		Logger _log;
		XMLVisitor _xml;

		// Tower-managed members
		std::list<Ship const *> _shipQueue;
		std::vector<std::pair<Point, Point>> _plannedMovements;

		// Managed Harbor instance
		Harbor * _harbor;

	protected:
		/*** Internal management methods ***/
		bool traceRoute(Point const & source, Point const & dest);
		bool planMovements();

		void applyPlannedMovements();

		bool replaceReservation(Ship const * const original,
					Ship const * const replacement);
		bool assignDock(Ship const * const s);
		void insertShip(Ship const * const s);
		void manageNewShips(unsigned const proba);

		Point chooseExit(Point const & source);
		void cleanExit();
		void manageOutgoingShip(Ship const * ship);

		void sleep(unsigned const milliseconds);

		// Ship creator
		Ship const * createShip();

		// Queue display
		void displayQueue();

	public:
		// Constructors & destructors
		Tower(Harbor * h);


		// Main management loop
		void cycle(unsigned const proba=50);
		void cycleOut();
};

#endif // TOWER_HPP_INCLUDED
