#include <iostream>
#include "Enumerator.h"
using namespace std;

class TrafficLights:public Enumerator<TrafficLights>{
    public:
    /*add to use Enumerator<TrafficLights> constructors */
    using Enumerator<TrafficLights>::Enumerator;
};

class Green:public TrafficLights::EnumEntry<1,Green>{
    public:static register_enum certificate;/*must add to register Green into TrafficLights or else won't compile*/
};

/*physically registers*/
Green::register_enum Green::certificate;

class Red:public TrafficLights::EnumEntry<2,Red>{
    public: static register_enum certificate;
};
Red::register_enum Red::certificate;


class Color:public Enumerator<Color>{
    public:
        using Enumerator<Color>::Enumerator;
    class Red: public Color::EnumEntry<1,Color::Red>{
        public: static register_enum certificate;
    };
    class Blue: public Color::EnumEntry<2,Color::Blue>{
        public: static register_enum certificate;
    };
};

Color::Red::register_enum Color::Red::certificate;
Color::Blue::register_enum Color::Blue::certificate;

int main()
{
    /*if you try to create an Enumerator without overloading it will not compile*/
    //TrafficLights light2; //BAD!!
    TrafficLights light = Green(); //Good
    cout << "traffic light = " << (string)light << endl;
    light = Red();
    cout << "traffic light = " << (string)light << endl;

    Color color = Color::Red();
    cout << "Color = " << (string)color << endl;
    color = Color::Blue();
    cout << "Color = " << (string)color << endl;

    /* output:
    **  traffic light = Green
    **  traffic light = Red
    **  Color = Color::Red
    **  Color = Color::Blue
    */

    return 0;
}

/* a few last notes to consider
**  -add a boolean template to check the removal of all preceding texts and "::"
**   example: instead of getting Color::Blue i will get Blue
**  -make sure that each EnumEntry has a unique int,
**   maybe by declaring a type using the parent template parameter E and the template parameter VAL
**   it will be possible to make sure people use a unique int
*/
