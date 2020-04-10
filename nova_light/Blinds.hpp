#ifndef BLINDS_H
#define BLINDS_H

class Blinds {
public:
    virtual void open() = 0;

    virtual void close() = 0;

    virtual void setPercentage(int percentage) = 0;

    virtual int getPercentage() = 0;

protected:
    int _currentPercentage{0};
};

#endif // BLINDS_H