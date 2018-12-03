#include <time.h>
#include <iostream>
#include <string>
#include <cstdio>


// Utils for measuring time, etc...
bool readFileContent(char *buffer, size_t sizeRead, FILE *fp);



#include <chrono>
//This class prints a message (and the ellapsed time) when its destructor is called (unless the message is empty)
//we may also reset the timer and get the current time now (since when the class was constructed)
class Timer {
public:
    Timer(const std::string &msg): st(msg) {reset();}
    ~Timer() {
        if(st.size()!=0)
            printNow(st);
    }
    void reset() { t0 = std::chrono::system_clock::now(); }
    void printNow(const std::string &st) const {
        std::cerr << "Time to " << st << " : "<< getTimeNow() << "\n";
    }
    double getTimeNow() const {
        auto  t1 = std::chrono::system_clock::now();;
        //clock_gettime(CLOCK_REALTIME, &t1);
        return convertTimeMsecs(diff(t0,t1))/1000;
    }
private:
    typedef std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::duration<long long int, std::ratio<1, 1000000000> > > TIME_T;
    std::string st;
    TIME_T t0;

    double convertTimeMsecs(const std::chrono::duration<double> td) const {
        return td.count()*1000;
    }

    std::chrono::duration<double> diff(TIME_T  start, TIME_T  end) const
    {
        return end-start;
    }

};
