#ifndef PROPERTYEDIT_H_
#define PROPERTYEDIT_H_

class PropertyEdit {
public:
    PropertyEdit() {};
    virtual ~PropertyEdit() {};
    virtual bool increment() = 0; // returns if the value was changed
    virtual bool decrement() = 0; // returns if the value was changed
    virtual void accept() = 0;
    virtual void cancel() = 0;
    virtual void display() = 0;
};

#endif /* PROPERTYEDIT_H_ */
