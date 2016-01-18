#include "AnalogReader.h"

AnalogReader::AnalogReader(int minValue, 
        int maxValue,
        int lowWater,
        int highWater,
        int granularity,
        int consecutiveReadingsRequired):
    m_minValue(minValue),
    m_maxValue(maxValue),
    m_lowWater(lowWater),
    m_highWater(highWater),
    m_granularity(granularity),
    m_consecutiveReadingsRequired(consecutiveReadingsRequired),
    m_currentValue(UNINITIALIZED),
    m_potentialNewValue(UNINITIALIZED),
    m_consecutiveReadingsRemaining(consecutiveReadingsRequired)
{
}

int AnalogReader::addReading(int reading)
{
    /* Special case for adding the first reading */
    if (m_currentValue == UNINITIALIZED && m_potentialNewValue == UNINITIALIZED)
    {
        m_currentValue = m_potentialNewValue = reading;
    }
    if (reading <= m_currentValue + m_granularity && reading >= m_currentValue - m_granularity)
    {
        /* Treat reading the same as the current value */
        return normalizeValue(m_currentValue);
    }

    if (reading <= m_potentialNewValue + m_granularity && reading >= m_potentialNewValue - m_granularity)
    {
        /* Another reading of a potential value */
        m_consecutiveReadingsRemaining--;
    }
    else
    {
        /* A reading from a new value, reset */
        m_potentialNewValue = reading;
        m_consecutiveReadingsRemaining = m_consecutiveReadingsRequired;
        m_consecutiveReadingsRemaining--;
    }

    if (m_consecutiveReadingsRemaining <= 0)
    {
        /* Met criteria, promote the potential value to currentValue */
        m_currentValue = m_potentialNewValue;
        m_consecutiveReadingsRemaining = m_consecutiveReadingsRequired;
    }

    return normalizeValue(m_currentValue);
}

int AnalogReader::getCurrentValue()
{
    return normalizeValue(m_currentValue);
}

int AnalogReader::normalizeValue(int value)
{
    int returnValue = value;

    if (value > m_highWater)
    {
        returnValue = m_maxValue;
    }
    else if (value < m_lowWater)
    {
        returnValue = m_minValue;
    }

    return returnValue;
}
