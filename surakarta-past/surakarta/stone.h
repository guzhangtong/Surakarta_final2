#ifndef STONE_H
#define STONE_H

class Stone
{
public:
    Stone(){}
    // 重载 == 运算符以比较两个 Stone 对象是否相等
    bool operator==(const Stone& other) const {
        return _x == other._x &&
               _y == other._y &&
               _id == other._id &&
               _dead == other._dead &&
               _color == other._color;
    }
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
