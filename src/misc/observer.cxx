
#include "observer.hpp"


event::~event() {
}


void event::consume() {
    consumed = true;
}


observer::~observer() {
    for (auto s : subjects) {
        s->observers.remove(this);
    }
}


void observer::notify(event *event) {
}


void observer::subscribe(subject *s) {
    subjects.push_back(s);
    s->observers.push_back(this);
}


void observer::unsubscribe(subject *s) {
    subjects.remove(s);
    s->observers.remove(this);
}


subject::~subject() {
    for (auto o : observers) {
        o->subjects.remove(this);
    }
}


void subject::notify(event *event) {
    event->source = this;
    event->consumed = false;
    for (auto o : observers) {
        o->notify(event);
        if (event->consumed) {
            event->handler = o;
            return;
        }
    }
}

