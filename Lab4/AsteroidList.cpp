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

AsteroidList::AsteroidList() {}

AsteroidList::AsteroidList(const AsteroidList& src)
{
	insertAfter(&head, src);
}

AsteroidList::~AsteroidList() {
	clear();
}

void AsteroidList::pushFront(Asteroid e) {
	AsteroidListItem* newItem = new AsteroidListItem(e);
	newItem->setNext(head.getNext());
	head.setNext(newItem);
}

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

int AsteroidList::size() const {
	int sz = 0;
	const AsteroidListItem* iterator = &head;
	while (iterator->hasNext()) {
		sz++;
		iterator = iterator->getNext();
	}
	return sz;
}

AsteroidListItem* AsteroidList::beforeBegin() {
	return &head;
}

const AsteroidListItem* AsteroidList::beforeBegin() const {
	return &head;
}

AsteroidListItem* AsteroidList::begin() {
	return head.getNext();
}

const AsteroidListItem* AsteroidList::begin() const {
	return head.getNext();
}

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

AsteroidListItem* AsteroidList::end() {
	return nullptr;
}

const AsteroidListItem* AsteroidList::end() const {
	return nullptr;
}

AsteroidListItem* AsteroidList::insertAfter(AsteroidListItem* prev, Asteroid e) {
	AsteroidListItem* newItem = new AsteroidListItem(e);
	AsteroidListItem* oldNext = prev->setNext(newItem);
	prev->getNext()->setNext(oldNext);
	return newItem;
}

AsteroidListItem* AsteroidList::insertAfter(AsteroidListItem* prev, const AsteroidList& others) {
	AsteroidListItem* iterator = prev;
	AsteroidListItem* jointRight = prev->getNext();
	for (AsteroidListItem* i = (AsteroidListItem*) others.begin(); i != others.end(); i = i->getNext()) {
		AsteroidListItem* newItem = new AsteroidListItem(i->getData());
		iterator->setNext(newItem);
		iterator = iterator->getNext();
	}
	iterator->setNext(jointRight);
	return iterator;
}

AsteroidListItem* AsteroidList::eraseAfter(AsteroidListItem* prev) {
	if (!prev->hasNext())
		return nullptr;
	AsteroidListItem* toDelete = prev->setNext(prev->getNext()->getNext());
	delete toDelete;
	return prev->getNext();
}

void AsteroidList::clear() {
	while (head.hasNext()) {
		AsteroidListItem* toDelete = head.setNext(head.getNext()->getNext());
		delete toDelete;
	}
}

AsteroidList& AsteroidList::operator=(const AsteroidList& src) {
	clear();
	insertAfter(&head, src);
	return *this;
}
