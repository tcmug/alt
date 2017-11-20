#ifndef _MISC_OBSERVER_HPP_
#define _MISC_OBSERVER_HPP_

#include <list>

class Subject;
class Observer;

class Event {

    friend class Observer;
    friend class Subject;

    private:

        bool consumed;

    public:

        virtual ~Event();

        Subject *source;
        Observer *handler;

        void consume();

};


class Observer {

    friend class Subject;

    private:

        std::list <Subject*> _subjects;

    public:

        virtual ~Observer();

        void subscribe(Subject *o);
        void unsubscribe(Subject *subject);

        virtual void notify(Event *event);

};


class Subject {

    friend class Observer;

    private:

        std::list <Observer *> _observers;

    public:

        virtual ~Subject();
        virtual void notify(Event *event);

};


#endif
