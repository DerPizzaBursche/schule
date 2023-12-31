#include <iostream>
//#include <list>
using namespace std;


template<typename T>
class IList{ //oder auch BaseList
//erster ansatz mit Listen Bibliothek verworfen ==> ohne Listen probieren mit struct versuchen
//public:
// Ausgangspunkt Liste als Basis
//    virtual bool empty()=0;
//    virtual bool endpos()=0;
//    virtual list<T> reset()=0;
//    virtual list<T> advance()=0;
//    virtual T elem() = 0;
//    virtual list<T> insert()=0;
//    virtual list<T> remove()=0;  //soll delete sein

//protected:
//    list<T> Elements;

//};
    //ansatz über strukturen(struct) mit generischer Nutzbarkeit
    public:
        virtual bool empty()=0;
        virtual bool endpos()=0;
        virtual void reset()=0;
        virtual void advance()=0;
        virtual T elem() = 0;
        virtual void insert(T& element)=0; //T& ==> Referenz  //nur variablen möglich keine direkten werte
        virtual void remove()=0;  
        virtual ~IList() {}
};

template <typename T>
class List : public IList<T>{
    private:
        struct Item {
            T data; //Datenmember
            Item* next; //nach folgender Member des Struct
            Item(T& d) : data(d), next(nullptr){} //Konstruktor
            //d ==> benannte var ==> daten werden direkt in data Kopiert
        };
        Item* first; // Zeiger auf anfang der Liste
        Item* current; //Zeiger auf aktueller Eintrag
    public :
        List(): first(nullptr), current(nullptr){}
        bool empty(){
            return first == nullptr;
        }
        bool endpos(){
           return current == nullptr;
        }
        void reset(){
            current = first;
        }
        void advance(){
            if(current){
                current = current -> next; //current wird auf den zeiger next gesetzt
            }
        }
        T elem(){
            if (current) {
                return current->data;
            } else {
                throw runtime_error("element is not defined");
            }
        }
        void insert(T& element){
            Item* NewItem = new Item(element);
            if(!current){
                NewItem->next = first; //zeiger auf nachfolgenden Member setzen, der vorher der erste war
                first = NewItem; //zeiger vom ersten auf den neu hinzugefügten setzen
                current = first; //current zeiger auf den ersten zeiger setzen
            }else{
                if(current == first){ //wenn current und first gleich sind
                    NewItem->next = first;  //Zeiger auf den nächsten Eintrag auf den vom vorherigen ersten setzen
                    first = NewItem; // Zeiger vom Ersten Eitnrag auf den neuen Eintrag setzen
                }else{
                    Item* prev = first; //vorheriges Item instanzieren
                    while(prev->next!=current){ //aktualisiert
                        prev = prev->next;
                    }
                    prev->next = NewItem;
                    NewItem->next = current;
                }
            }
        }
        //aka Delete
        void remove(){
          if(!current){
            throw runtime_error("No element to delete");
          }  else{
              Item* tmp = current;
              if(current == first){
                  first = current->next;
                  current = first;
              }else{
                  Item* prev = first;
                  while(prev->next != current){
                      prev = prev->next;
                  }
                  prev->next = current->next;
                  current = current->next;
              }
              delete tmp;
          }
        }
        ~List(){
            while(first){
                Item* tmp = first;
                first = first->next;
                delete tmp;
            }
        }
};




int main()
{
    List<int> t;
    for(int i = 0; i < 10; i++){
        t.insert(i);
    }
    t.reset();
    while(!t.endpos()){
       int test = t.elem();
       cout << test << endl;;
       t.advance();
    }
   t.~List();

    return 0;
}
