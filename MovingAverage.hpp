#ifndef __MOVINGAVERAGE_HPP__
#define __MOVINGAVERAGE_HPP__

namespace mobo
{
    class MovingAverage
    {
        public:
            MovingAverage(int iWindowSize) : windowSize(iWindowSize), buffer(new float[windowSize]), start(0), end(0), pointCount(0) { }
            virtual ~MovingAverage() { if(buffer) delete [] buffer; }

            float operator[](int iIndex) {
                int i = (iIndex + start) % windowSize;
                return buffer[i];
            }

            void push_back(float iPoint) {
                buffer[end] = iPoint;
                end++;
                if(pointCount < windowSize) {
                    pointCount++;
                } else {
                    start++;
                }
                start %= windowSize;
                end %= windowSize;
            }

            float avg() {
                if(!pointCount) return 0.0;
                int n = pointCount;
                float sum = 0.0;
                while(n--) sum += (*this)[n];
                return sum / (float) pointCount;
            }

            float wma() {
                if(!pointCount) return 0.0;
                int n = pointCount;
                float sum = 0.0;
                while(n--) sum += (*this)[n] * float(pointCount - n - 1);
                return 2.0f * sum / float(pointCount * (pointCount + 1));
            }

            int windowSize;
            float *buffer;
            int start, end;
            int pointCount;
    };
}

#endif
