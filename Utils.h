#pragma once
#include <SFML/Graphics.hpp>
#include <sstream>

const int     MaxInt32	= (std::numeric_limits<int>::max)();
const double  MaxDouble = (std::numeric_limits<double>::max)();
const double  MinDouble = (std::numeric_limits<double>::min)();
const float   MaxFloat	= (std::numeric_limits<float>::max)();
const float   MinFloat	= (std::numeric_limits<float>::min)();
const float   EpsilonF = 0.00001f;//std::numeric_limits<float>::epsilon(); //use a practical value

const float   Pi		= 3.14159f;
const float   TwoPi		= Pi * 2;
const float   HalfPi	= Pi / 2;
const float   QuarterPi = Pi / 4;

template<typename T>
inline int Sign(T value) {
	return (T{} < value) - (value < T{});
}

template<>
inline int Sign<float>(float value) {
	if (std::isnan(value)) return 0; // Optionally handle NaN as 0 or another special value
	if (std::isinf(value)) return (value > 0.0f) ? 1 : -1;
	return (0.0f < value) - (value < 0.0f);
}

template<>
inline int Sign<double>(double value) {
	if (std::isnan(value)) return 0; // Optionally handle NaN as 0 or another special value
	if (std::isinf(value)) return (value > 0.0) ? 1 : -1;
	return (0.0 < value) - (value < 0.0);
}



template<typename T>
inline void NumberToString(T num, std::string& str)
{
	std::ostringstream ss;
	ss << num;
	str += ss.str();
}

inline bool IsZero(float val)
{
	return ((-MinFloat < val) && (val < MinFloat));
}

inline double DegsToRads(double degs)
{
	return TwoPi * (degs / 360.0);
}

inline float RadToDeg(float rad)
{
	return(rad * (180 / Pi));
}

inline float Fmod(float numer, float denom)
{
	return fmod(numer, denom);
}

template <typename T>
T Max(const T& a, const T& b)
{
	return (a < b ? b : a);
}

template <typename T>
T Min(const T & a, const T & b)
{
	return (a < b ? a : b);
}

template <typename T>
inline T Clamp(const T& value, const T& lower, const T& upper)
{
	return Min(upper, Max(lower, value));
}

inline bool NearZero(float val, float epsilon = EpsilonF/*0.0001f*/)
{
	if (fabs(val) <= epsilon)
	{
		return true;
	}
	else
	{
		return false;
	}
}

inline float dot(const sf::Vector2f& lv, const sf::Vector2f& rv)
{
	return lv.x* rv.x + lv.y * rv.y;
}

inline float length(const sf::Vector2f& source)
{
	//return std::sqrt(dot(source, source)); // same as std::hypot
	return std::hypot(source.x, source.y);
}

inline float magnitude(const sf::Vector2f& source)
{
	return length(source);
}

inline double distance(const sf::Vector2f& v1, const sf::Vector2f& v2)
{

	float ySeparation = v2.y - v1.y;
	float xSeparation = v2.x - v1.x;

	return std::hypot(xSeparation, ySeparation);
	//return sqrt(ySeparation * ySeparation + xSeparation * xSeparation);
}

[[nodiscard]] inline sf::Vector2f normalize(sf::Vector2f source)
{
	//float length = std::sqrt(dot(source, source));
	float length = std::hypot(source.x, source.y);
	if (length > std::numeric_limits<float>::epsilon())
		source /= length;
	return source;
}

[[nodiscard]] inline sf::Vector2f truncate(sf::Vector2f v, float max)
{
	if (length(v) > max)
	{
		v = normalize(v);
		v *= max;
	}
	return v;
}

inline sf::Vector2f GetReverse(sf::Vector2f v)
{
	return sf::Vector2f(-v.x, -v.y);
}


//------------------------ Sign ------------------------------------------
//
//  returns positive if v2 is clockwise of this vector,
//  minus if anticlockwise (Y axis pointing down, X axis to right)
//------------------------------------------------------------------------

enum { clockwise = 1, anticlockwise = -1 };

inline int sign(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
	if (v1.y * v2.x > v1.x * v2.y)
		return anticlockwise;
	else
		return clockwise;
}

