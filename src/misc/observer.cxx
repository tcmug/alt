
#include "Observer.h"


Event::~Event() {
}


void Event::consume() {
    _consumed = true;
}


Observer::~Observer() {
    for (auto s : _subjects) {
        s->_observers.remove(this);
    }
}


void Observer::notify(Event *Event) {
}


void Observer::subscribe(Subject *s) {
    _subjects.push_back(s);
    s->_observers.push_back(this);
}


void Observer::unsubscribe(Subject *s) {
    _subjects.remove(s);
    s->_observers.remove(this);
}


Subject::~Subject() {
    for (auto o : _observers) {
        o->_subjects.remove(this);
    }
}


void Subject::notify(Event *event) {
    event->_source = this;
    event->_consumed = false;
    for (auto o : _observers) {
        o->notify(event);
        if (event->_consumed) {
            event->_handler = o;
            return;
        }
    }
}

