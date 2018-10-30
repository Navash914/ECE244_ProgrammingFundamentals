/*
 * This file was developed for the Fall 2018 instance of ECE244 at the University of Toronto.
 * Creator: Matthew J. P. Walker
 * Supervised by: Tarek Abdelrahman
 */

#ifndef ECE244_ASTEROID_LIST_HPP
#define ECE244_ASTEROID_LIST_HPP

//#include <galaxy-explorer/Asteroid.hpp>
#include "Asteroid.hpp"

/**
 * An "item" within an `AsteroidList`. Contains things like the next pointer, and the Asteroid that is in the list.
 */
class AsteroidListItem {
public:
	AsteroidListItem();

	AsteroidListItem(Asteroid a);

	~AsteroidListItem();

	/**
	 * Get the Asteroid that this list item
	 */
	      Asteroid& getData()       { return *data; }
	const Asteroid& getData() const { return *data; }

	/**
	 * Get the list item that comes after this one
	 */
	      AsteroidListItem* getNext()       { return next; }
	const AsteroidListItem* getNext() const { return next; }

	/**
	 * Set `next` to `elli`, and return the old value of `next`.
	 */
	AsteroidListItem* setNext(AsteroidListItem* elli) { AsteroidListItem* old = next; next = elli; return old; }
	bool hasNext() const { return next != nullptr; }
private:
	AsteroidListItem(const AsteroidListItem& src) { *this = src; }
	AsteroidListItem& operator=(const AsteroidListItem& rhs) {
		next = nullptr;
		delete data;
		data = new Asteroid(*rhs.data);
		return *this;
	}

	AsteroidListItem* next;
	Asteroid* data;
};

/**
 * A singly-linked list of entities. Manages it's own memory, and has an interface not
 * completely dissimilar to the standard library's `std::forward_list`, however simplified
 * for the purposes of teaching. `std::forward_list` and the standard library's container library
 * are fairly well-designed, and so these simplifications have resulted in a few compromises,
 * so not all functionality is present.
 *
 * Functions are ordered by a suggested implementation order, except for the destructor and copy constructor
 */
class AsteroidList {
public:
	/**
	 * Creates an empty list
	 */
	AsteroidList();

	/**
	 * Creates an independent copy of `src`. This means that there is NO shared data or pointers.
	 * The functions in this class are listed in a suggested order of implementation,
	 * except for this one and the destructor (because you should put all your constructors together).
	 */
	AsteroidList(const AsteroidList& src);

	/**
	 * Frees all memory associated with this list
	 * The functions in this class are listed in a suggested order of implementation,
	 * except for this one and the copy constructor (because you should put all your constructors together).
	 */
	~AsteroidList();

	/**
	 * Add an asteroid at the beginning of this list
	 */
	void pushFront(Asteroid e);

	/**
	 * Return a reference to the first asteroid
	 * This function may have ANY BEHAVIOUR if this list is empty
	 */
	      Asteroid& front()      ;
	const Asteroid& front() const;

	/**
	 * Are there no entities in this list?
	 */
	bool isEmpty() const;

	/**
	 * The number of entities in this list
	 */
	int size() const;

	/**
	 * Return an item corresponding to the position immediately before the first asteroid.
	 * If this list is empty, a value equal to `beforeEnd()` must be returned.
	 * If this list is in a valid state, this function must always succeed.
	 */
	      AsteroidListItem* beforeBegin()      ;
	const AsteroidListItem* beforeBegin() const;

	/**
	 * Return an item corresponding to the first asteroid.
	 * If this list is empty, a value equal to `end()` must be returned.
	 * If this list is in a valid state, this function must always succeed.
	 */
	      AsteroidListItem* begin()      ;
	const AsteroidListItem* begin() const;

	/**
	 * Return an item corresponding to the position of the last asteroid.
	 * If this list is empty, a value equal to `beforeBegin()` must be returned.
	 * If this list is in a valid state, this function must always succeed.
	 */
	      AsteroidListItem* beforeEnd()      ;
	const AsteroidListItem* beforeEnd() const;

	/**
	 * Return an item corresponding to the position immediately after the last asteroid.
	 * If this list is empty, a value equal to `begin()` must be returned.
	 * If this list is in a valid state, this function must always succeed.
	 */
	      AsteroidListItem* end()      ;
	const AsteroidListItem* end() const;

	/**
	 * Add a single asteroid to this list, in the position immediately after `prev`.
	 * returns the item that was inserted
	 */
	AsteroidListItem* insertAfter(AsteroidListItem* prev, Asteroid e);

	/**
	 * Add independent copies of the entities in `others` to this list, placed immediately after `insertion_point`,
	 * in the same order as `others`.
	 * Returns the last item that was inserted, or `insertion_point' if `others' is empty.
	 */
	AsteroidListItem* insertAfter(AsteroidListItem* insertion_point, const AsteroidList& others);

	/**
	 * Given a position in this list, delete the following asteroid.
	 * Returns the list item that followed the erased item, or end() if there wasn't one.
	 * This function may have ANY BEHAVIOUR if there is no asteroid after `prev`.
	 */
	AsteroidListItem* eraseAfter(AsteroidListItem* prev);

	/**
	 * Make this list empty, free all nodes.
	 */
	void clear();

	/**
	 * Makes this list an independent copy of `src`. This means that there is NO shared data or pointers.
	 */
	AsteroidList& operator=(const AsteroidList& src);

private:
	AsteroidListItem head;
};

#endif /* ECE244_ASTEROID_LIST_HPP */
