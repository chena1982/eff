#pragma once

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#define DECLARE_CLASS(c)\
private:\
	friend class boost::serialization::access;\
	template<class Archive>\
	void serialize(Archive & ar, const unsigned int version)\

#define MEMBER(m)\
ar & m;
