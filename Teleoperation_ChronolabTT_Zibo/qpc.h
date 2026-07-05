#ifndef QPC_H
#define QPC_H

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#include <process.h>
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <sys/time.h>
#endif

#include <iostream>

/**
 * Wrapper for the Query Performance Counter, gives a simple and precise measure of time
 */
class QPC
{

public:
    QPC()
    {
#if defined(_WIN32) || defined(_WIN64)
        m_counterstart = 0;
        m_pcfreq = 0.0;
        LARGE_INTEGER li;
        if(!QueryPerformanceFrequency(&li))
            std::cout << "QueryPerformanceFrequency failed!" << std::endl;

        m_pcfreq = double(li.QuadPart)/1000.0;

        QueryPerformanceCounter(&li);
        m_counterstart = li.QuadPart;
        m_tictime = m_counterstart/m_pcfreq;
#elif defined(unix) || defined(__unix) || defined(__unix__)
        gettimeofday(&m_counterstart, NULL);
#endif
    }

    double getms()
    {
#if defined(_WIN32) || defined(_WIN64)
        LARGE_INTEGER li;
        QueryPerformanceCounter(&li);
        return double(li.QuadPart-m_counterstart)/m_pcfreq;
#elif defined(unix) || defined(__unix) || defined(__unix__)
        struct timeval getms_time;

        gettimeofday(&getms_time, NULL);

        return double(((getms_time.tv_sec-m_counterstart.tv_sec) * 1000 + (getms_time.tv_usec-m_counterstart.tv_usec)/1000.0) + 0.5);
#endif
    }

    double gets()
    {
        return getms()/1000.0;
    }

    void reset()
    {
        LARGE_INTEGER li;
        QueryPerformanceCounter(&li);
        m_counterstart = li.QuadPart;
    }

    void tic()
    {
        m_tictime = gets();
    }

    double toc()
    {
        double t = gets();
        double r = t - m_tictime;
        m_tictime = t; //tic()
        return r;
    }

    #if defined(_WIN32) || defined(_WIN64)
    void usleep(unsigned __int64 ticks)
    {
        LARGE_INTEGER frequency;
        LARGE_INTEGER currentTime;
        LARGE_INTEGER endTime;

        QueryPerformanceCounter(&endTime);

        // Ticks in microseconds (1/1000 ms)
        QueryPerformanceFrequency(&frequency);
        endTime.QuadPart += (ticks * frequency.QuadPart) / (1000ULL * 1000ULL);

        do
        {
            SwitchToThread();

            QueryPerformanceCounter(&currentTime);
        } while (currentTime.QuadPart < endTime.QuadPart);
    }
    #endif

private:
#if defined(_WIN32) || defined(_WIN64)
    double m_pcfreq;
    __int64 m_counterstart;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    struct timeval m_counterstart;
#endif

    double m_tictime;
};

#endif // QPC_H

