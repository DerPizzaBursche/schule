#ifndef GENERICLIST_H
#define GENERICLIST_H
#include <igenericlist.h>
template <typename T>
class GenericList : public IGenericList {
public:
    GenericList(int initialCapacity = 10);
    ~GenericList();

    bool empty() const override;
    bool endpos() const override;
    void reset() override;
    void advance() override;
    void insert(T element) override;
    T elem() const override;
    void remove() override;
    void invert() override;

private:
    void expandArray();
    void shiftElements(int index);

    int size;
    int capacity;
    T* array;
    int currentPosition;
};

#endif  // GENERICLIST_H
