#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <list>


class subject;
class observer;


class event {

    friend class observer;
    friend class subject;

    private:

        bool consumed;

    public:

        virtual ~event();

        subject *source;
        observer *handler;

        void consume();

};


class observer {

    friend class subject;

    private:

        std::list <subject*> subjects;

    public:

        virtual ~observer();

        void subscribe(subject *o);
        void unsubscribe(subject *subject);

        virtual void notify(event *event);

};


class subject {

    friend class observer;

    private:

        std::list <observer *> observers;

    public:

        virtual ~subject();
        void notify(event *event);

};


#endif
