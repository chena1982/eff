/******************************************************************************
created:	2017-07-07   22:38
file path:	d:\EFF\Include\EFFGUI\EFFGUICoord.h
author:		ChenA

purpose:
******************************************************************************/
#ifndef __EFFGUICoord_2017_07_07__
#define __EFFGUICoord_2017_07_07__

EFFGUI_BEGIN

template<typename T>
struct TPoint
{
    T left;
    T top;

    TPoint() :
        left(0),
        top(0)
    {
    }

    TPoint(T const& _left, T const& _top) :
        left(_left),
        top(_top)
    {
    }

    TPoint(TPoint const& _obj) :
        left(_obj.left),
        top(_obj.top)
    {
    }

    TPoint& operator -= (TPoint const& _obj)
    {
        left -= _obj.left;
        top -= _obj.top;
        return *this;
    }

    TPoint& operator += (TPoint const& _obj)
    {
        left += _obj.left;
        top += _obj.top;
        return *this;
    }

    TPoint operator - (TPoint const& _obj) const
    {
        return TPoint(left - _obj.left, top - _obj.top);
    }

    TPoint operator + (TPoint const& _obj) const
    {
        return TPoint(left + _obj.left, top + _obj.top);
    }

    TPoint& operator = (TPoint const& _obj)
    {
        left = _obj.left;
        top = _obj.top;
        return *this;
    }

    template<typename U>
    TPoint& operator = (TPoint<U> const& _obj)
    {
        left = _obj.left;
        top = _obj.top;
        return *this;
    }

    bool operator == (TPoint const& _obj) const
    {
        return ((left == _obj.left) && (top == _obj.top));
    }

    bool operator != (TPoint const& _obj) const
    {
        return !((left == _obj.left) && (top == _obj.top));
    }

    void clear()
    {
        left = top = 0;
    }

    void set(T const& _left, T const& _top)
    {
        left = _left;
        top = _top;
    }

    void swap(TPoint& _value)
    {
        TPoint tmp = _value;
        _value = *this;
        *this = tmp;
    }

    bool empty() const
    {
        return ((left == 0) && (top == 0));
    }
};

template<typename T>
struct TSize
{
    T width;
    T height;

    TSize() :
        width(0),
        height(0)
    {
    }

    TSize(T const& _width, T const& _height) :
        width(_width),
        height(_height)
    {
    }

    TSize(TSize const& _obj) :
        width(_obj.width),
        height(_obj.height)
    {
    }

    TSize& operator -= (TSize const& _obj)
    {
        width -= _obj.width;
        height -= _obj.height;
        return *this;
    }

    TSize& operator += (TSize const& _obj)
    {
        width += _obj.width;
        height += _obj.height;
        return *this;
    }

    TSize operator - (TSize const& _obj) const
    {
        return TSize(width - _obj.width, height - _obj.height);
    }

    TSize operator + (TSize const& _obj) const
    {
        return TSize(width + _obj.width, height + _obj.height);
    }

    TSize& operator = (TSize const& _obj)
    {
        width = _obj.width;
        height = _obj.height;
        return *this;
    }

    template<typename U>
    TSize& operator = (TSize<U> const& _obj)
    {
        width = _obj.width;
        height = _obj.height;
        return *this;
    }

    bool operator == (TSize const& _obj) const
    {
        return ((width == _obj.width) && (height == _obj.height));
    }

    bool operator != (TSize const& _obj) const
    {
        return !((width == _obj.width) && (height == _obj.height));
    }

    void clear()
    {
        width = height = 0;
    }

    void set(T const& _width, T const& _height)
    {
        width = _width;
        height = _height;
    }

    void swap(TSize& _value)
    {
        TSize tmp = _value;
        _value = *this;
        *this = tmp;
    }

    bool empty() const
    {
        return ((width == 0) && (height == 0));
    }

    std::string print() const
    {
        std::ostringstream stream;
        stream << *this;
        return stream.str();
    }
};

template<typename T>
struct TRect
{
    T left;
    T top;
    T right;
    T bottom;

    TRect() :
        left(0),
        top(0),
        right(0),
        bottom(0)
    {
    }

    TRect(T const& _left, T const& _top, T const& _right, T const& _bottom) :
        left(_left),
        top(_top),
        right(_right),
        bottom(_bottom)
    {
    }

    TRect(TRect const& _obj) :
        left(_obj.left),
        top(_obj.top),
        right(_obj.right),
        bottom(_obj.bottom)
    {
    }

    TRect& operator -= (TRect const& _obj)
    {
        left -= _obj.left;
        top -= _obj.top;
        right -= _obj.right;
        bottom -= _obj.bottom;
        return *this;
    }

    TRect& operator += (TRect const& _obj)
    {
        left += _obj.left;
        top += _obj.top;
        right += _obj.right;
        bottom += _obj.bottom;
        return *this;
    }

    TRect operator - (TRect const& _obj) const
    {
        return TRect(left - _obj.left, top - _obj.top, right - _obj.right, bottom - _obj.bottom);
    }

    TRect operator + (TRect const& _obj) const
    {
        return TRect(left + _obj.left, top + _obj.top, right + _obj.right, bottom + _obj.bottom);
    }

    TRect& operator = (TRect const& _obj)
    {
        left = _obj.left;
        top = _obj.top;
        right = _obj.right;
        bottom = _obj.bottom;
        return *this;
    }

    template<typename U>
    TRect& operator = (TRect<U> const& _obj)
    {
        left = _obj.left;
        top = _obj.top;
        right = _obj.right;
        bottom = _obj.bottom;
        return *this;
    }

    bool operator == (TRect const& _obj) const
    {
        return ((left == _obj.left) && (top == _obj.top) && (right == _obj.right) && (bottom == _obj.bottom));
    }

    bool operator != (TRect const& _obj) const
    {
        return !((left == _obj.left) && (top == _obj.top) && (right == _obj.right) && (bottom == _obj.bottom));
    }

    T width() const
    {
        return right - left;
    }

    T height() const
    {
        return bottom - top;
    }

    void clear()
    {
        left = top = right = bottom = 0;
    }

    void set(T const& _left, T const& _top, T const& _right, T const& _bottom)
    {
        left = _left;
        top = _top;
        right = _right;
        bottom = _bottom;
    }

    void swap(TRect& _value)
    {
        TRect tmp = _value;
        _value = *this;
        *this = tmp;
    }

    bool empty() const
    {
        return ((left == 0) && (top == 0) && (right == 0) && (bottom == 0));
    }

    bool inside(const TRect<T>&  _value) const
    {
        return ((_value.left >= left) && (_value.right <= right) && (_value.top >= top) && (_value.bottom <= bottom));
    }

    bool intersect(const TRect<T>&  _value) const
    {
        return ((_value.left <= right) && (_value.right >= left) && (_value.top <= bottom) && (_value.bottom >= top));
    }

    bool inside(const TPoint<T>&  _value) const
    {
        return ((_value.left >= left) && (_value.left <= right) && (_value.top >= top) && (_value.top <= bottom));
    }
};

template<typename T>
struct TCoord
{
    T left;
    T top;
    T width;
    T height;

    TCoord() :
        left(0),
        top(0),
        width(0),
        height(0)
    {
    }

    TCoord(T const& _left, T const& _top, T const& _width, T const& _height) :
        left(_left),
        top(_top),
        width(_width),
        height(_height)
    {
    }

    TCoord(TCoord const& _obj) :
        left(_obj.left),
        top(_obj.top),
        width(_obj.width),
        height(_obj.height)
    {
    }

    TCoord(TPoint<T> const& _point, TSize<T> const& _size) :
        left(_point.left),
        top(_point.top),
        width(_size.width),
        height(_size.height)
    {
    }

    TCoord& operator -= (TCoord const& _obj)
    {
        left -= _obj.left;
        top -= _obj.top;
        width -= _obj.width;
        height -= _obj.height;
        return *this;
    }

    TCoord& operator += (TCoord const& _obj)
    {
        left += _obj.left;
        top += _obj.top;
        width += _obj.width;
        height += _obj.height;
        return *this;
    }

    TCoord operator - (TCoord const& _obj) const
    {
        return TCoord(left - _obj.left, top - _obj.top, width - _obj.width, height - _obj.height);
    }

    TCoord operator - (TPoint<T> const& _obj) const
    {
        return TCoord(left - _obj.left, top - _obj.top, width, height);
    }

    TCoord operator - (TSize<T> const& _obj) const
    {
        return TCoord(left, top, width - _obj.width, height - _obj.height);
    }

    TCoord operator + (TCoord const& _obj) const
    {
        return TCoord(left + _obj.left, top + _obj.top, width + _obj.width, height + _obj.height);
    }

    TCoord operator + (TPoint<T> const& _obj) const
    {
        return TCoord(left + _obj.left, top + _obj.top, width, height);
    }

    TCoord operator + (TSize<T> const& _obj) const
    {
        return TCoord(left, top, width + _obj.width, height + _obj.height);
    }

    TCoord& operator = (TCoord const& _obj)
    {
        left = _obj.left;
        top = _obj.top;
        width = _obj.width;
        height = _obj.height;
        return *this;
    }

    template< typename U >
    TCoord& operator = (TCoord<U> const& _obj)
    {
        left = _obj.left;
        top = _obj.top;
        width = _obj.width;
        height = _obj.height;
        return *this;
    }

    TCoord& operator = (TPoint<T> const& _obj)
    {
        left = _obj.left;
        top = _obj.top;
        return *this;
    }

    TCoord& operator = (TSize<T> const& _obj)
    {
        width = _obj.width;
        height = _obj.height;
        return *this;
    }

    bool operator == (TCoord const& _obj) const
    {
        return ((left == _obj.left) && (top == _obj.top) && (width == _obj.width) && (height == _obj.height));
    }

    bool operator != (TCoord const& _obj) const
    {
        return !((left == _obj.left) && (top == _obj.top) && (width == _obj.width) && (height == _obj.height));
    }

    T right() const
    {
        return left + width;
    }

    T bottom() const
    {
        return top + height;
    }

    void clear()
    {
        left = top = width = height = 0;
    }

    void set(T const& _left, T const& _top, T const& _width, T const& _height)
    {
        left = _left;
        top = _top;
        width = _width;
        height = _height;
    }

    void swap(TCoord& _value)
    {
        TCoord tmp = _value;
        _value = *this;
        *this = tmp;
    }

    bool empty() const
    {
        return ((left == 0) && (top == 0) && (width == 0) && (height == 0));
    }

    TPoint<T> point() const
    {
        return TPoint<T>(left, top);
    }

    TSize<T> size() const
    {
        return TSize<T>(width, height);
    }

    bool inside(const TPoint<T>&  _value) const
    {
        return ((_value.left >= left) && (_value.left <= right()) && (_value.top >= top) && (_value.top <= bottom()));
    }

	static TCoord<T> parse(const effStringA & _value)
	{
		TCoord<T> result;

		std::istringstream stream(_value.c_str());
		stream >> result.left >> result.top >> result.width >> result.height;
		if (stream.fail())
		{
			return TCoord<T>();
		}
		else
		{
			int item = stream.get();
			while (item != -1)
			{
				if (item != ' ' && item != '\t')
					return TCoord<T>();
				item = stream.get();
			}
		}
		return result;
	}
};

typedef TPoint<int> IntPoint;
typedef TPoint<float> FloatPoint;

typedef TSize<int> IntSize;
typedef TSize<float> FloatSize;

typedef TRect<int> IntRect;
typedef TRect<float> FloatRect;

typedef TCoord<int> IntCoord;
typedef TCoord<float> FloatCoord;


EFFGUI_END

#endif