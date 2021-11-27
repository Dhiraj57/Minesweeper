
template<class T>
T clamp(T value, T low, T high)
{
    if(value < low)
    {
        return low;
    }
    else if (value > high)
    {   
        return high;
    }
    
    return value;
}