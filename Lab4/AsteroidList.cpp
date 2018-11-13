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

AsteroidList::AsteroidList(const AsteroidList& src) {
    insertAfter(&head, src);
}

AsteroidList::~AsteroidList() {
    clear();
}

void AsteroidList::pushFront(Asteroid e) {
    insertAfter(&head, e);
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
    return !head.hasNext();
}

int AsteroidList::size() const {
    int sz = 0;
    for (const AsteroidListItem* it = begin(); it != end(); 
            it = it->getNext(), sz++);
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
    AsteroidListItem* it = beforeBegin();
    while (it->hasNext())
            it = it->getNext();
    return it;
}

const AsteroidListItem* AsteroidList::beforeEnd() const {
    const AsteroidListItem* it = beforeBegin();
    while (it->hasNext())
            it = it->getNext();
    return it;
}

AsteroidListItem* AsteroidList::end() {
    return nullptr;
}

const AsteroidListItem* AsteroidList::end() const {
    return nullptr;
}

AsteroidListItem* AsteroidList::insertAfter(AsteroidListItem* prev, Asteroid e) {
    if (prev == nullptr)
        return nullptr;
    AsteroidListItem* newItem = new AsteroidListItem(e);
    newItem->setNext(prev->setNext(newItem));
    return newItem;
}

AsteroidListItem* AsteroidList::insertAfter(AsteroidListItem* prev, const AsteroidList& others) {
    for (AsteroidListItem* it = (AsteroidListItem*) others.begin();
        it != others.end(); it = it->getNext()
    )
        prev = insertAfter(prev, it->getData());
    return prev;
}

AsteroidListItem* AsteroidList::eraseAfter(AsteroidListItem* prev) {
    if (!prev->hasNext())
            return nullptr;
    delete prev->setNext(prev->getNext()->getNext());
    return prev->getNext();
}

void AsteroidList::clear() {
    while (!isEmpty())
        eraseAfter(&head);
}

AsteroidList& AsteroidList::operator=(const AsteroidList& src) {
    clear();
    insertAfter(&head, src);
    return *this;
}
