//#include <galaxy-explorer/AsteroidList.hpp>
#include "AsteroidList.hpp"

AsteroidListItem::AsteroidListItem() {
	this->next = nullptr;
	this->data = nullptr;
}

AsteroidListItem::AsteroidListItem(Asteroid a) {
	this->next = nullptr;
	this->data = new Asteroid(a);
}

AsteroidListItem::~AsteroidListItem() {
	delete data;
}

// head.next is automatically assigned to nullptr
// via the AsteroidListItem default constructor
AsteroidList::AsteroidList() = default;

AsteroidList::AsteroidList(const AsteroidList& src)
{
	insertAfter(&head, src);
}

AsteroidList::~AsteroidList() {
	clear();
}

// Adds new list item at the front of the list
void AsteroidList::pushFront(Asteroid e) {
	AsteroidListItem* newItem = new AsteroidListItem(e);
	newItem->setNext(head.getNext());
	head.setNext(newItem);
}

// Returns data from the first list item
Asteroid& AsteroidList::front() {
	if (!isEmpty())
		return head.getNext()->getData();
	else
		return *(Asteroid*)nullptr;
}

const Asteroid& AsteroidList::front() const {
	if (!isEmpty())
		return head.getNext()->getData();
	else
		return *(const Asteroid*)nullptr;
}

bool AsteroidList::isEmpty() const {
	return head.getNext() == nullptr;
}

// Calculates and returns size of list
int AsteroidList::size() const {
	int sz = 0;
	const AsteroidListItem* iterator = &head;
	while (iterator->hasNext()) {
		sz++;
		iterator = iterator->getNext();
	}
	return sz;
}

// Returns pointer to element before the first (ie head)
AsteroidListItem* AsteroidList::beforeBegin() {
	return &head;
}

const AsteroidListItem* AsteroidList::beforeBegin() const {
	return &head;
}

// Returns pointer to first element of the list
AsteroidListItem* AsteroidList::begin() {
	return head.getNext();
}

const AsteroidListItem* AsteroidList::begin() const {
	return head.getNext();
}

// Returns last element of the list
AsteroidListItem* AsteroidList::beforeEnd() {
	AsteroidListItem* iterator = &head;
	while (iterator->hasNext())
		iterator = iterator->getNext();
	return iterator;
}

const AsteroidListItem* AsteroidList::beforeEnd() const {
	const AsteroidListItem* iterator = &head;
	while (iterator->hasNext())
		iterator = iterator->getNext();
	return iterator;
}

// Returns pointer to element after the last (ie nullptr)
AsteroidListItem* AsteroidList::end() {
	return nullptr;
}

const AsteroidListItem* AsteroidList::end() const {
	return nullptr;
}

// Create new list item with asteroid and add to list after prev
AsteroidListItem* AsteroidList::insertAfter(AsteroidListItem* prev, Asteroid e) {
	AsteroidListItem* newItem = new AsteroidListItem(e);
	AsteroidListItem* oldNext = prev->setNext(newItem);
	prev->getNext()->setNext(oldNext);
	return newItem;
}

// Create new list items from source list and add to list after prev
AsteroidListItem* AsteroidList::insertAfter(AsteroidListItem* prev, const AsteroidList& others) {
	AsteroidListItem* iterator = prev;
	AsteroidListItem* joinAtEnd = prev->getNext();
	for (AsteroidListItem* i = (AsteroidListItem*) others.begin();
			i != others.end(); i = i->getNext()
	)
		iterator = insertAfter(iterator, i->getData());
	iterator->setNext(joinAtEnd);
	return iterator;
}

// Delete element from list after prev
AsteroidListItem* AsteroidList::eraseAfter(AsteroidListItem* prev) {
	if (!prev->hasNext())
		return nullptr;
	AsteroidListItem* toDelete = prev->setNext(prev->getNext()->getNext());
	delete toDelete;
	return prev->getNext();
}

// Removes all elements from list
void AsteroidList::clear() {
	while (head.hasNext()) {
		AsteroidListItem* toDelete = head.setNext(head.getNext()->getNext());
		delete toDelete;
	}
}

// Clears list and replaces it with copy of list from src
AsteroidList& AsteroidList::operator=(const AsteroidList& src) {
	clear();
	insertAfter(&head, src);
	return *this;
}
