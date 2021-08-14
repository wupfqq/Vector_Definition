#include <iostream>
#include <memory>

template <typename T>
class Vector{
private:  T* mass;
    size_t size;
    size_t capt;

public:    Vector(){               //создается вектор по умолчанию, по аналогии создаются остальные консрукторы вектора
        mass=new T[1];
        size=0;
        capt=1;
    }

    void reserve(size_t n){
        if(n<=capt) return;
        T* newr=reinterpret_cast<T*>(new int8_t[n*sizeof(T)]);
        size_t i=0;
        //1
        /*try{                                 //защита от исключений при копировании(то же самое нужно в resize,push_back)
                                             //можно через std::uninitialized_copy()
        for(i=0;i<size;++i){
            new(newr+i)T(mass[i]);  //placement new
          }}
        catch(...){
          for(size_t j=0;j<i;++j)
            {
              (newr+j)->~T();}  //явный вызов деструктора
              delete[]reinterpret_cast<int8_t*>(mass);   //на "обычные байты "не вызываются деструкторы
              throw;
        }*/
        //2
        //блок кода 1-2 можно переписать используя uninitialized copy
        try {
            std::uninitialized_copy(mass,mass+size,newr);
        } catch (...) {
            delete[]reinterpret_cast<int8_t*>(mass);   //на "обычные байты "не вызываются деструкторы
            throw;
        }
        for(size_t i=0;i<size;++i)
            (mass+i)->~T();
        delete[]reinterpret_cast<int8_t*>(mass);
        capt=n;
        mass=newr;
    }



    void resize(size_t n, const T& value=T()){                   //default value
        if(n>capt)reserve(n);
        size_t i=0;

        try{
            for(i=size;i<n;++i){
                new(mass+i)T(value);
            }}
        catch(...){
            for(size_t j=0;j<i;++j)
            {
                (mass+j)->~T();} //явный вызов деструктора

                //      delete[]reinterpret_cast<int8_t*>(mass+size);  если был сделан reserve - удалить выделенное им место
                throw;
        }

        if(n<size)
            size=n;
    }




    void push_back(const T& value){
        if(capt==size)
            reserve(2*size);
        try{
            new(mass+size)T(value);
            size++;}
        catch(...){
            (mass+size)->~T();
            //delete[]reinterpret_cast<int8_t*>(mass+size);  если был сделан reserve - удалить выделенное им место
        }
    }

    T& pop_back(){
        T& temp=mass[size-1];
        --size;
        (mass+size)->~T();
        return temp;
    }

    T& operator [](size_t ind){
        return mass[ind];
    }

    const T& operator [](size_t ind)const {
        return mass[ind];
    }

    T& at(size_t ind){
        if(ind>=size)
            throw std::out_of_range("index is out of range");
        else {
            return mass[ind];
        }
    }

    const T& at(size_t ind)const{
        if(ind>=size)
            throw std::out_of_range("index is out of range");
        else {
            return mass[ind];
        }
    }
};



template<>
class Vector<bool>{   //тип каждого элементы булевского массива std::Bit_referense
    uint8_t* starr;    //добавить другие операции(add, const методы и т.д.)

    struct bitReference{
        uint8_t posit;
        uint8_t* base;

        bitReference(uint8_t a,uint8_t* b): posit(a), base(b){}

        bitReference& operator =(bool exp){
            (exp)?(*base)|=((uint8_t)1<<posit):(*base)&=~((uint8_t)1<<posit);
            return *this;
        }

        operator bool() const{
            return *base&(1<<posit);
        }
    };

public:  Vector(){starr=new uint8_t;}


bitReference operator[](uint8_t ind){
        uint8_t npos=ind % 8;
        uint8_t* newp=starr+npos/8;
        return bitReference(npos,newp);
    }

};



int main(){
    Vector<int> t;
    t.push_back(13);
    t.push_back(90);
    t.push_back(666);
  //  std::cout<<t.pop_back()<<t.pop_back();

    Vector<bool> b;
    b[4]=1;
    b[7]=1;
    b[67]=0;
    b[10]=1;
    b[666]=1;
    b[87]=1;
    b[132]=1;

    std::cout<<b[7];


}
