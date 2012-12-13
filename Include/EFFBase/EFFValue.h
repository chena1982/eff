/********************************************************************
	created:	21:11:2010   16:21
	file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFValue.h
	author:		ChenA
	
	purpose:	
*********************************************************************/
#ifndef __EFFValue_2010_11_21__
#define __EFFValue_2010_11_21__


EFFBASE_BEGIN

class BaseValue;

class Value        // Value handle
{
public:
	Value(BaseValue const & bv)
		: v(bv.clone())
	{}

	Value(Value const & rhs)
		: v(rhs.v ? rhs.v->clone() : 0)
	{}

	explicit Value(BaseValue * bv = 0)
		: v(bv)
	{}

	~Value()
	{
		delete v;
	}

	Value & operator=(const Value & rhs)
	{
		// this is not a typical pimpl assignment, but a set()
		if (v)
		{
			if (rhs.v)
			{ // fine, all v's exist
				v->set(rhs);
			}
			else
			{ // the other v doesn't exist, so we must delete our own :-(
				BaseValue * old = v;
				v = 0;
				delete old;
			}
		}
		else
		{ // we don't have a v, so just copy the other
			v = (rhs.v ? rhs.v->clone() : 0);
		}

		return *this;
	}

	template <typename PlainT>
	PlainT get() const
	{
		if (v)
		{
			RealValue<PlainT> const & rv
				= dynamic_cast<RealValue<PlainT> const &>(*v);
			return rv;        // uses conversion operator
		}
		else
		{
			return PlainT();
		}
	}

	std::string toString() const
	{
		if (v)
		{
			return v->toString();
		}
		else
		{
			return string();
		}
	}

private:
	BaseValue * v;
};


class BaseValue
{
public:
	virtual ~BaseValue() {}

	virtual BaseValue * clone() const = 0;

	virtual string toString() const = 0;

	virtual void set(Value const & v) = 0;
	// no get()!

};

template <typename PlainT>
class RealValue : public BaseValue
{
public:
	RealValue(PlainT v)
		: val(v) {}

	RealValue * clone() const
	{
		return new RealValue(*this);
	}

	string toString() const
	{
		ostringstream os;
		os << val;
		return os.str();
	}

	operator PlainT() const // conversion to plain type
	{
		return val;
	}

	void RealValue<PlainT>::set(const Value & v)
	{
		val = v.get<PlainT>();
	}


private:
	PlainT val;
};

EFFBASE_END

#endif