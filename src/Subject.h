/*
 * MVC example of GTKmm program
 *
 * Subject class.  Abstract class for Observer Pattern
 *
 *
 *  Created by Jo Atlee on 06/07/09.
 *  Copyright 2009 UW. All rights reserved.
 *
 */


#ifndef MVC_SUBJECT_H
#define MVC_SUBJECT_H

#include <set>


class Observer;

class Subject {
public:
       void subscribe( Observer* );
       void unsubscribe( Observer* );
       virtual ~Subject() = 0;            //need polymorphic to dynamic cast. Also, should derive from subject class and never purely instantiate subject.

protected:
       void notify();

private:
       typedef std::set< Observer* > Observers;
       Observers observers_;
}; // Subject


#endif
