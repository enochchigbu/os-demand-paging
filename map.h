//    Single Programmer Affidavit
//    I the undersigned promise that the submitted assignment is my own work. While I was
//    free to discuss ideas with others, the work contained is my own. I recognize that
//    should this not be the case; I will be subject to penalties as outlined in the course
//    syllabus.
//    Name: Enoch Chigbu
//    Red ID: 827078077
//
//    Name: Farhan Talukder
//    Red ID: 827061320

#ifndef MAP_H
#define MAP_H

class Map
{
public:
    Map()
    {
        this->frame = -1;
        this->valid = false;
    }

    void setValid(int frame)
    {
        this->frame = frame;
        this->valid = true;
    }

    void setInvalid()
    {
        this->frame = -1;
        this->valid = false;
    }

    bool isValid()
    {
        return this->valid;
    }

    int getFrame()
    {
        return this->frame;
    }

private:
    int frame;
    bool valid; // false = invalid, true = valid
};

#endif