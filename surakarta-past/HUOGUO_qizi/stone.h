#ifndef STONE_H
#define STONE_H

class Stone
{
public:
    Stone();
    //~Stone();
    enum TYPE{CAT,DOG};
    int _x;
    int _y;
    int _id;
    bool _dead;
    bool _color;
    void init(int id)
    {
        _id=id;
        _dead=false;
        _color=id<12;
        if(id<6)
        {
            _x=id;
            _y=0;
        }
        if(id>=6&&id<12)
        {
            _x=id%6;
            _y=1;
        }
        if(id>=12&&id<18)
        {
            _x=id%6;
            _y=4;
        }
        if(id>=18)
        {
            _x=id%6;
            _y=5;
        }
    }

};

#endif // STONE_H
