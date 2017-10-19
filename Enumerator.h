#ifndef _ENUMERATOR_H_
#define _ENUMERATOR_H_

#include <string>
#include <set>
#include <cstdlib>
#include <stdexcept>

/*a way to get the class name using type_id*/
#ifdef __GNUG__
#include <cstdlib>
#include <memory>
#include <cxxabi.h>

template <typename C>
std::string _demangle(const char* name){
    int status; // some arbitrary value to eliminate the compiler warning

    // enable c++11 by passing the flag -std=c++11 to g++
    std::unique_ptr<char, void(*)(void*)> res {abi::__cxa_demangle(name, NULL, NULL, &status), std::free };
        return (status==0) ? res.get() : name ;
}
#else
// does nothing if not g++
template <typename C>
std::string _demangle(const char* name) {
    return name;
}
#endif

/*you can get the name by overloading the C*/
template <typename C>
std::string get_object_name(){
    return _demangle<C>(typeid(C).name());
}

/*you can automatically get the object type name by passing the object itself*/
template <typename C>
std::string get_object_name(const C& obj){
    return _demangle<C>(typeid(C).name());
}

template <typename E>
class Enumerator
{
    public:
        class EnumEntryBase{
        protected:
            virtual EnumEntryBase* clone() = 0;
        public:
            EnumEntryBase(){}
            virtual ~EnumEntryBase(){}
            virtual operator int ()const = 0;
            virtual operator std::string()const = 0;
            virtual bool operator==(int other){
                return (int)*this == other;
            }

            virtual bool operator!=(int other){
                return (int)*this != other;
            }
        };

        template <int VAL,typename T>
        class Enum_Register{
        private:
            Enum_Register(const Enum_Register&) = delete;
            virtual Enum_Register& operator=(const Enum_Register&) = delete;
        public:
            Enum_Register(){
                Enumerator::register_enum(new T);
            }
            virtual ~Enum_Register(){}

            void check_registration(){}
        };

        template <int VAL,typename T>
        class EnumEntry:public Enumerator::EnumEntryBase{
            protected:
                virtual Enumerator::EnumEntryBase* clone(){ return new T; }
                //virtual void register_object
            public:
                typedef Enum_Register<VAL,T> register_enum;
                static const std::string & NAME(){
                    static std::string _NAME = get_object_name<T>();
                    return _NAME;
                }

                EnumEntry(){ T::certificate.check_registration(); }
                virtual ~EnumEntry(){}
                virtual operator int ()const final{ return VAL; }
                virtual operator std::string()const final{ return NAME(); }
        };

        static Enumerator<E>::EnumEntryBase& parse(const std::string& val){
            for(auto e:ENUM_LIST())
                if(*e == val)
                    return e.get();
            throw std::invalid_argument("value " + val + " does not exist in enum " + NAME());
        }

        static Enumerator<E>::EnumEntryBase& parse(int val){
            for(auto e:ENUM_LIST())
                if(*e == val)
                    return *e;
            throw std::invalid_argument("value " + std::to_string(val) + " does not exist in enum " + NAME());
        }

        explicit Enumerator(const std::string& value)
        :_current_value(&(parse(value)))
        {}

        explicit Enumerator(int value)
        :_current_value(&(parse(value)))
        {}

        Enumerator(const Enumerator& enumerator)
        :_current_value(enumerator._current_value)
        {}

        Enumerator(const EnumEntryBase& num)
        :_current_value(&(parse((int)num)))
        {}

        virtual Enumerator& operator=(const Enumerator& other){
            if(this != &other){
                _current_value = other._current_value;
            }
            return *this;
        }

        virtual Enumerator& operator=(const EnumEntryBase& value){
            _current_value = &parse((int)value);
            return *this;
        }

        virtual bool operator==(int other){
            return *_current_value == other;
        }

        virtual bool operator!=(int  other){
            return *_current_value != other;
        }

        virtual operator int()const{
            return *_current_value;
        }

        virtual operator std::string()const{
            return *_current_value;
        }

        EnumEntryBase& get()const{
            return *_current_value;
        }

        virtual ~Enumerator(){}
    private:
        static std::set<std::shared_ptr<EnumEntryBase>>& ENUM_LIST(){
            static std::set<std::shared_ptr<EnumEntryBase>> _ENUM_LIST;
            return _ENUM_LIST;
        }

        static const std::string& NAME(){
            static std::string _NAME = get_object_name<E>();
            return _NAME;
        }

        static void register_enum(EnumEntryBase* enum_val){
            ENUM_LIST().insert(std::shared_ptr<EnumEntryBase>(enum_val));
        }

        EnumEntryBase* _current_value;
};

#endif // _ENUMERATOR_H_
