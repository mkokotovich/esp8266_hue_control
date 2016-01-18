#ifndef _ANALOG_READER_
#define _ANALOG_READER_

#define UNINITIALIZED -12345

class AnalogReader {
    public:
        /*
         * minValue - minimum valid value
         * maxValue - maximum valid value
         * lowWater - any value below this value will be treated as minValue
         * highWater - any value above this value will be treated as maxValue
         * granularity - the delta between two readings in order to be treated as seperate values
         * consecutiveReadings - The number of readings within a certain granularity required before updating currentValue
         */
        AnalogReader(int minValue,
                     int maxValue,
                     int lowWater,
                     int highWater,
                     int granularity,
                     int consecutiveReadingsRequired);
        ~AnalogReader() {};

        /* Adds reading, returns current value */
        int addReading(int reading);

        int getCurrentValue();
        
    private:
        int normalizeValue(int value);

        int m_minValue;
        int m_maxValue;
        int m_lowWater;
        int m_highWater;
        int m_granularity;
        int m_consecutiveReadingsRequired;

        int m_currentValue;
        int m_potentialNewValue;
        int m_consecutiveReadingsRemaining;

};

#endif
