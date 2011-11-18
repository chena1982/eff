/******************************************************************************
BigWorld Technology
Copyright BigWorld Pty, Ltd.
All Rights Reserved. Commercial in confidence.

WARNING: This computer program is protected by copyright law and international
treaties. Unauthorized use, reproduction or distribution of this program, or
any portion of this program, may result in the imposition of civil and
criminal penalties as provided by law.
******************************************************************************/

#ifndef MATH_EXTRA_HPP
#define MATH_EXTRA_HPP

#include "math_namespace.hpp"
#include "vector2.hpp"

BEGIN_BW_MATH


/**
 *	This class is used to represent a 1 dimensional range.
 */
class Range1D
{
public:
	void set( float minValue, float maxValue )
	{
		data_[0] = minValue;
		data_[1] = maxValue;
	}

	float & operator[]( int i ) 		{ return data_[i]; }
	float operator[]( int i ) const		{ return data_[i]; }

	float length() const				{ return data_[1] - data_[0]; }

	float midPoint() const				{ return (max_ + min_)/2.f; }

	void inflateBy( float value )
	{
		min_ -= value;
		max_ += value;
		max_ = std::max( min_, max_ );
	}

	bool contains( float pt ) const	{ return (min_ <= pt) && (pt <= max_); }
	bool contains( const Range1D & range ) const
		{ return (min_ <= range.min_) && (range.max_ <= max_); }

	float distTo( float pt ) const	{ return std::max( pt - max_, min_ - pt ); }

	union
	{
		float data_[2];
		struct { float min_, max_; };
	};
};


/**
 *	This class represents a rectangle in 2 dimensions.
 */
class Rect
{
public:
	Rect( float xMin, float yMin, float xMax, float yMax )
	{
		x_.set( xMin, xMax );
		y_.set( yMin, yMax );
	}

	Rect() {};

	float area() const	{ return x_.length() * y_.length(); }

	void inflateBy( float value )
	{
		x_.inflateBy( value );
		y_.inflateBy( value );
	}

	bool intersects( const Rect & rect ) const
	{
		return
			(rect.xMin_ <= xMax_) &&
			(xMin_ <= rect.xMax_) &&
			(rect.yMin_ <= yMax_) &&
			(yMin_ <= rect.yMax_);
	}

	bool contains( float x, float y ) const
	{
		return x_.contains( x ) && y_.contains( y );
	}

	float distTo( float x, float y ) const
	{
		return std::max( x_.distTo( x ), y_.distTo( y ) );
	}

	void setToUnion( const Rect & r1, const Rect & r2 )
	{
		xMin_ = std::min( r1.xMin_, r2.xMin_ );
		yMin_ = std::min( r1.yMin_, r2.yMin_ );
		xMax_ = std::max( r1.xMax_, r2.xMax_ );
		yMax_ = std::max( r1.yMax_, r2.yMax_ );
	}

	Range1D & range1D( bool getY )				{ return getY ? y_ : x_; }
	const Range1D & range1D( bool getY ) const	{ return getY ? y_ : x_; }

	float xMin() const							{ return xMin_; }
	float xMax() const							{ return xMax_; }
	float yMin() const							{ return yMin_; }
	float yMax() const							{ return yMax_; }

	void xMin(float value )						{ xMin_ = value; }
	void xMax(float value )						{ xMax_ = value; }
	void yMin(float value )						{ yMin_ = value; }
	void yMax(float value )						{ yMax_ = value; }

	const Range1D & xRange() const				{ return x_; }
	const Range1D & yRange() const				{ return y_; }

	union
	{
		struct { float xMin_, xMax_; };
		Range1D x_;
	};
	union
	{
		struct { float yMin_, yMax_; };
		Range1D y_;
	};
};


inline
bool watcherStringToValue( const char * valueStr, BW::Rect & rect )
{
	return sscanf( valueStr, "%f,%f,%f,%f",
			&rect.xMin_, &rect.yMin_,
			&rect.xMax_, &rect.yMax_ ) == 4;
}

inline
std::string watcherValueToString( const BW::Rect & rect )
{
	char buf[256];
	sprintf( buf, "%.3f, %.3f, %.3f, %.3f",
			rect.xMin_, rect.yMin_,
			rect.xMax_, rect.yMax_ );

	return buf;
}


END_BW_MATH


#endif // MATH_EXTRA_HPP
