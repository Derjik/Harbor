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

#ifndef HARBOR_HPP_INCLUDED
#define HARBOR_HPP_INCLUDED

#include <map>		// std::map
#include <set>		// std::set

#include "Ship.hpp"	// Ship
#include "Point.hpp"	// Point
#include "Logger.hpp"	// Logger, custom endl


/*
 * Represents the physical Harbor in memory, mapping Ship instances
 * to Point keys and managing their respective moves.
 */

class Harbor
{
	private:
		// Singleton pattern instance pointer
		static Harbor * _instance;


		// Used for logging purposes
		Logger _log;

		// Harbor's matrix (linking coordinates to Ship pointers)
		std::map<Point, Ship const *> _surface;
		// Reverse matrix (used for Ship location purposes)
		std::map<Ship const *, Point> _reverseSurface;
		// Harbor's width
		unsigned _width;
		// Harbor's height
		unsigned _height;

		// Harbor's entry points
		std::set<Point> _entryPoints;

		// Dock mapping (linking dock IDs to coordinates)
		std::map<unsigned, Point> _docks;
		// Dock reverse mapping (used for display purposes)
		std::map<Point, unsigned> _reverseDocks;

		// Dock reservation map (linking Ship names to dock IDs)
		std::map<std::string, unsigned> _reservations;

		// Docks available for reservation
		std::set<unsigned> _availableDocks;

		/*** Constructor & destructor ***/
		Harbor(unsigned const width=40, unsigned const height=20);
		virtual ~Harbor();

	protected:
		/*** Collisions-related methods ***/
		bool collision(Ship const * const s1, Ship const * const s2);

	public:
		/*** Singleton pattern-related methods ***/
		static Harbor * getInstance(unsigned const width=40,
						unsigned const height=20);
		static void deleteInstance();

		/*** Width & height related methods ***/
		unsigned width() const { return _width; }
		unsigned height() const { return _height; }

		/*** Surface-related methods ***/
		bool addShip(Ship const & s, Point const & p);
		bool addShip(Ship const * s, Point const & p);

		Ship const * getShipAt(Point const & p) const;
		Point getShipPosition(Ship const * s) const;

		bool moveShip(Point const & src, Point const & dst);

		bool removeShip(Point const & p);
		bool removeShip(Ship const * p);

		std::set<Point> const & entryPoints() const;

		std::map<Point, Ship const *> const & surface() const;
		std::map<Ship const *, Point> const & reverseSurface() const;


		/*** Docks-related methods ***/
		Point getDockPosition(unsigned const id) const;

		std::set<unsigned> const & availableDocks() const;

		bool reserveDock(unsigned const dockId, Ship const * const s);
		bool reserveDock(unsigned const dockId, Ship const & s);

		bool removeReservation(std::string const & s);

		unsigned getReservedDock(Ship const * const s) const;
		unsigned getReservedDock(Ship const & s) const;

		std::map<unsigned, Point> const & dockMap() const;
		std::map<Point, unsigned> const & reverseDockMap() const;

		/*** Display-related methods ***/
		void display() const;
};

/*
 * IMPORTANT DISCLAIMER:
 * The removeShip() method implemented in this class
 * does in NO WAY delete the removed Ship instance.
 * External controllers should avoid memory leaks by
 * ensuring the removed Ships are properly deleted
 * if they are no longer used in the simulation.
 */

#endif // HARBOR_HPP_INCLUDED
