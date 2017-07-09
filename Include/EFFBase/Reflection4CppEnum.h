/**
* @Author zrf (zhaorufei@amica.com.cn)
* @date   2009-05-26
* @brief  Hybrid of C/C++ Macro and template technique to implement
*         C#-style reflection support for Enum in C++.
*
* @ChangeLog 2010-09-24:
* add is_bit_flag, bitflags_to_string, bitflags_parse
* add support to define inside namespace/class
*
* Detailed description of file:
*   With this component, you can:
*         * iterator the enum's enumerator: value_begin
*         * iterator the enum's string representation: str_begin
*         * determine whether a int/long long is a defined enum item: is_defined
*         * determine whether a string is a defined enum item(can be
*           case-insensitive): is_defined
*         * get enumerator's string representation by it's value
*         * get enumerator's value by it's string representation(can be
*           case-insensitive)
*         * enums defined in the old way can *NOT* take benefit from it. of course
*
* Usage:
*  1. The old way:
*  enum WeekDay { Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday };
*
*  2. The reflect way:
*  #define ALL_THE_WEEK(x) \
*  ENUM_ITEM(x, Sunday)\
*  ENUM_ITEM(x, Monday)\
*  ENUM_ITEM(x, Tuesday)\
*  ENUM_ITEM(x, Wednesday)\
*  ENUM_ITEM(x, Thursday)\
*  ENUM_ITEM(x, Friday)\
*  ENUM_ITEM(x, Saturday)
*
*  DEF_ENUM(WeekDay, ALL_THE_WEEK)
*  =============================================================
*  You can also use microsoft's enum extension of:(warning 4480)
*  enum WeekDay: short { Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday };
*  If you want to disable pc-lint warning about it(-e10 to disable error
*  10 temporarily)
*
*  DEF_ENUM_WITH_TYPE(WeekDay, short, ALL_THE_WEEK)
*
*  You can also specify enum's value by:
*  #define ALL_THE_WEEK(x) \
*  ENUM_ITEM_VALUE(x, Sunday, 1)\
*  ENUM_ITEM_VALUE(x, Monday, 3)\
*  ENUM_ITEM_VALUE(x, Tuesday, Sunday)\
*  ENUM_ITEM_VALUE(x, Wednesday, 10)\
*  ENUM_ITEM_VALUE(x, Thursday, 7)\
*  ENUM_ITEM(x, Friday, 8)\
*  ENUM_ITEM_VALUE(x, Saturday, 12)
*
*  DEF_ENUM(WeekDay, ALL_THE_WEEK)
*
*  reflection on enum:
*  printf("Enum %d : %s\n",  Sunday, EnumHelper<WeekDay>::to_string(Sunday) );
*  #define TEST_ENUM "monday"
*  printf("Is [" TEST_ENUM "] defined? %s\n",  EnumHelper<WeekDay>::is_defined(TEST_ENUM, true)?"True": "False" );
*  printf("Is 100 a defined WeekDay: %s\n", EnumHelper<WeekDay>::is_defined(100)? "True": "False" );
*
*  printf(TEST_ENUM "'s value: %d\n",  EnumHelper<WeekDay>::parse(TEST_ENUM, true) );
*
*  for(EnumHelper<WeekDay>::const_value_iterator it = EnumHelper<WeekDay>::value_begin();
*      it != EnumHelper<WeekDay>::value_end(); ++it)
*  {
*      printf("Enum value: %d, string: %s\n", *it, EnumHelper<WeekDay>::to_string(*it) );
*  }
*
* Compiler compliance:
*   + Comeau C/C++ 4.3.10.1 (Oct  6 2008 11:28:09) for ONLINE_EVALUATION_BETA2
*   + VC2003 (/W4 /WX) Microsoft (R) 32-bit C/C++ Optimizing Compiler Version 13.10.6030 for 80x86
*   + VC2008 (/W4 /WX), with exception of warning 4996
*   + gcc -Wall (gcc (GCC) 4.2.3 (Ubuntu 4.2.3-2ubuntu7))
*   - Checked by PC-Lint online(9.00b)
*/
#ifndef REFLECT_CPP_ENUM_INCLUDE_GUARD
#define REFLECT_CPP_ENUM_INCLUDE_GUARD

//#include <string>
//#include <stdio.h> // for tolower
//#include <cstring>
#include <stdexcept> // for runtime_error

#define my_count_of(a)  (sizeof(a)/sizeof(a[0]))

// The name Enum is different from keyword enum only by first `E', so choose a more long name
template <typename Enum_T> class EnumHelper
{
private:
	struct GeneralEnumMetaData {
		Enum_T enum_value;
		const char * enum_str;
	};
public:

	/// @brief: To determine whether an string is a defined enumerator
	/// @param [In] enum_str: if NULL throw exception immediately
	/// @return: thrown runtime_error if enum_str is NULL, ref to C#'s Enum.IsDefined
	///   note even an obvious invalid enum string won't raise exception, just return false.
	static bool is_defined(const char * enum_str, bool ignore_case )
	{
		if(enum_str == NULL) throw std::runtime_error("enum_str is a NULL pointer");

		bool l_is_defined = false;
		from_string( l_is_defined, enum_str, ignore_case);
		return l_is_defined;
	}

	/// 
	/// @param [In] enum_str: if NULL return false immediately
	/// @return: thrown runtime_error if enum_str is NULL, ref to C#'s Enum.IsDefined
	///   note even an obvious invalid enum string won't raise exception, just return false.
	static inline bool is_defined(const char * enum_str )
	{
		return is_defined(enum_str, false);
	}

	/// @brief: Translate collection of bitflags to the corresponding
	///         comma-separated string
	/// @param e: Enum value
	/// @param [out] str: the comma separated(each comma is followed
	///         by a space) enum string.  e.g., "A, B, C"
	///         Or a digital string if not found.
    /// @exception : no
	/// @return: 
	/// @note DotNet ToString compatible
	/// [Flags]
	/// public enum MyColor {
	///     Unknown = 0,
	/// 	Blue = 2,
	/// 	Red = 1,
	/// 	Orange = 4,
	/// 	Cyan = 3,
	/// };
	///  ((MyColor)3).ToString() ==> Cyan
	///  ((MyColor)7).ToString() ==> Cyan, Orange
	///  ((MyColor)0).ToString() ==> Unknown
    static std::string & bitflags_to_string(Enum_T e, std::string & str)
    {
		str = "";

		Enum_T old_e = e;
		class Sorter {
			public: static bool compare_enum_value(const GeneralEnumMetaData & lhs
							,const GeneralEnumMetaData & rhs) {
						return ( (long long )lhs.enum_value < (long long)rhs.enum_value );
					}
		};
		VECTOR<GeneralEnumMetaData> all(all_enums_, all_enums_ + my_count_of(all_enums_) );
		sort(all.begin(), all.end(), Sorter::compare_enum_value);
		// For the first pass, consume all the values exactly equal to

		VECTOR<const char *> found_enums;
		for(VECTOR<GeneralEnumMetaData>::reverse_iterator it = all.rbegin(); it != all.rend(); ++it)
		{
			if( ((*it).enum_value & e) == (*it).enum_value )
			{
				found_enums.push_back( (*it).enum_str );
				e = static_cast<Enum_T>( e & ~(*it).enum_value );

				// DotNet: [Flags] enum { Unknown = 0, Blue = 1};  1 ==>
				// Blue, not contains 0.
				// While 0 ==> Unknown
				if( ! e ) break;
			}
		}

		if( found_enums.size() && 0 == e )
		{
			int i = found_enums.size() - 1;
			str += found_enums[i];
			for(i--; i >= 0; --i) {
				str += ", ";
				str += found_enums[i];
			}
			return str;
		}
		char digits[50] = {0};
		// long long is the longest possible integer data can be used as underlying data
		sprintf(digits, "%.*lld", sizeof(digits) - 1, static_cast<long long>(old_e) );
		str = digits;
		return str;
    }

    /// @brief: parse enum_str as a set of enum string which is separated by
    ///         separator, case-sensitivity is determined by ignore_case / Caller
    ///         should make sure the Enum_T is a bit-masked enum(i.e., the value is
    ///         1, 2, 4, 8...), the interleaving space is ignored
    /// @param separator: the default is ',', caller should make sure the
    ///                   separator is not a valid identifier character.
    /// @return: the bitwise-ORed value of every successfuly parsed enum item.
    /// @exception : thrown runtime_error if enum_str is NULL or failed to parse according
    /// to the string values and ignore_case(.NET Enum.Parse compatible)
    static Enum_T bitflags_parse(const char * enum_str, bool ignore_case = false, char separator = ',' )
    {
		if(enum_str == NULL) throw std::runtime_error("enum_str is NULL");

		Enum_T e = static_cast<Enum_T>(0);
		istringstream is(enum_str);
		string single;
		while( getline(is, single, separator ) )
		{
			single.erase(remove_if(single.begin(), single.end(), ::isspace ), single.end());
			Enum_T single_e;
			from_string( single_e, single.c_str(), ignore_case);
			e = static_cast<Enum_T>(e | single_e);
		}
		return e;
    }

	/// @brief: determine whether the every enumerator is bitflag such
	///         as 1, 2 ,4 , 8. Note 0 is NOT a valid bit-flag
	/// @note: this function may make no sense to you since c# also do
	///        NOT warn about enum value 3 even /warn:4
    static bool is_bitflags()
	{
		return is_bit_flag_;
	}

	static inline bool is_defined(int enum_value) throw()
	{
		return generic_is_defined(enum_value);
	}
	static inline bool is_defined(Enum_T enum_value) throw()
	{
		return generic_is_defined(enum_value);
	}
	static inline bool is_defined(long long int enum_value) throw()
	{
		return generic_is_defined(enum_value);
	}
	/// parse enum_str as a enum's literal string, case-sensitivity is depends on ignore_case
	/// @return: thrown runtime_error if enum_str is NULL or failed to parse 
	/// @exceptions runtime_error if enum_str is NULL
	static Enum_T parse(const char * enum_str, bool ignore_case )
	{
		if(enum_str == NULL) throw std::runtime_error("enum_str is a NULL pointer");

		Enum_T e;
		from_string( e, enum_str, ignore_case);
		return e;
	}

	/// parse enum_str as a enum's literal string, case-sensitive
	/// @return: thrown runtime_error if enum_str is NULL or enum_str is not a 
	///  defined enum item(comparison is case-sensitive)
	static Enum_T parse(const char * enum_str)
	{
		if(enum_str == NULL) throw std::runtime_error("enum_str is a NULL pointer");

		Enum_T e;
		from_string( e, enum_str, false);
		return e;
	}

	/// mimic the VECTOR<T>::const_iterator
	class const_value_iterator {
		friend class EnumHelper<Enum_T>;
	public:
		Enum_T operator*() const { return m_ptr->enum_value; }
		bool operator==(const const_value_iterator & rhs ) const { return m_ptr == rhs.m_ptr; }
		bool operator!=(const const_value_iterator & rhs ) const { return !(m_ptr == rhs.m_ptr); }
		const_value_iterator & operator++() { ++m_ptr; return *this; }
		const_value_iterator & operator--() { --m_ptr; return *this; }
	private: // forbid explicitly
		const_value_iterator & operator++(int) { return *this; }
		GeneralEnumMetaData * m_ptr;
		const_value_iterator(GeneralEnumMetaData * p): m_ptr(p) { }
	};

	class const_str_iterator {
		friend class EnumHelper<Enum_T>;
	public:
		const char * operator*() const { return m_ptr->enum_str; }
		bool operator==(const const_str_iterator & rhs ) const { return m_ptr == rhs.m_ptr; }
		bool operator!=(const const_str_iterator & rhs ) const { return !(m_ptr == rhs.m_ptr); }
		const_str_iterator & operator++() { ++m_ptr; return *this; }
		const_str_iterator & operator--() { --m_ptr; return *this; }
	private: // forbid explicitly
		const_str_iterator & operator++(int) { return *this; }
		const_str_iterator & operator--(int) { return *this; }
		GeneralEnumMetaData * m_ptr;
		const_str_iterator(GeneralEnumMetaData * p): m_ptr(p) { }
	};

	/// mimic VECTOR<T>::begin(), end()
	/// do *NOT* try to provide the rbegin/rend version
	static const_str_iterator str_begin() throw()
	{
		return const_str_iterator( &all_enums_[0] );
	}

	static const_str_iterator str_end() throw()
	{
		return const_str_iterator( EnumHelper<Enum_T>::all_enums_ + my_count_of(EnumHelper<Enum_T>::all_enums_)  );
	}

	static const_value_iterator value_begin() throw()
	{
		return const_value_iterator( &all_enums_[0] );
	}

	static const_value_iterator value_end() throw()
	{
		return const_value_iterator( EnumHelper<Enum_T>::all_enums_ + my_count_of(EnumHelper<Enum_T>::all_enums_)  );
	}

	/// @param [In] e: for undefined e, return NULL
	static const char * to_string(Enum_T e) throw()
	{
		for(size_t i = 0; i < my_count_of(EnumHelper<Enum_T>::all_enums_) ; ++i)
		{
			if( all_enums_[i].enum_value == e)
				return all_enums_[i].enum_str;
		}
		return NULL;
	}

	/// if e is not a defined enum, the digits will be stored in str, and return false;
	static bool to_string(Enum_T e, std::string & str) throw()
	{
		for(size_t i = 0; i < my_count_of(EnumHelper<Enum_T>::all_enums_) ; i++)
		{
			if( all_enums_[i].enum_value == e)
			{
				str = all_enums_[i].enum_str;
				return true;
			}
		}
		char digits[50] = {0};
		// long long is the longest possible integer data can be used as underlying data
		sprintf(digits, "%.*lld", sizeof(digits) - 1, static_cast<long long>(e) );
		str = digits;
		return false;
	}

	/// get the index for the enum in declaration order
	/// @param [In] enum_str: return -1 if NULL
	/// @param [In] ignore_case: whether the comparison of enum string is case-sensitive
	/// @return [0, num_enumerators_) if found, -1 if not a defined item
	///         or enum_str is NULL
	static inline int index_of(const char * enum_str, bool ignore_case)
	{
		if(NULL == enum_str) return -1;

		for(size_t i = 0; i < my_count_of(EnumHelper<Enum_T>::all_enums_) ; i++)
		{
			if( are_equal(enum_str, EnumHelper<Enum_T>::all_enums_[i].enum_str, ignore_case) )
			{
				return i;
			}
		}
		return -1;
	}

	/// get the index for the enum in declaration order
	/// @param [In] enum_str: throw runtime_error if NULL, case-sensitive
	/// @return [0, num_enumerators_) if found, -1 if not a defined item
	/// @exception runtime_error will be thrown if enum_str is null
	static inline int index_of(const char * enum_str)
	{
		return index_of(enum_str, false);
	}

	static inline int index_of(int enum_value) throw()
	{
		return generic_index_of<int>(enum_value);
	}

	static inline int index_of(long long int enum_value) throw()
	{
		return generic_index_of<long long int>(enum_value);
	}

	static inline int index_of(Enum_T enum_value) throw()
	{
		return generic_index_of<Enum_T>(enum_value);
	}

	/// @brief: get number of enumerators defined for this enum
	/// @note:  not define a size or length function to confuse user
	static inline int count(void)
	{
		return num_enumerators_;
	}

	/// @brief: get the name of the enum type(no namespace/class prefix,
	///only the enum type name itself
	static inline const char * get_type_name(void) {
		return type_name_;
	}

private:

	static const int num_enumerators_;
	static const char * type_name_;

	/// get the index for the enum in declaration order, if more than one enumerator
	/// has the same value, the index for the first match is return
	/// @param [In] enum_value: 
	/// @return [0, num_enumerators_) if found, -1 if not a defined item
	template <typename integrate_T> static inline int generic_index_of(integrate_T enum_value) throw()
	{
		for(size_t i = 0; i < my_count_of(EnumHelper<Enum_T>::all_enums_) ; i++)
		{
			if( EnumHelper<Enum_T>::all_enums_[i].enum_value == static_cast<Enum_T>(enum_value) )
			{
				return i;
			}
		}
		return -1;
	}

	static GeneralEnumMetaData all_enums_[];
	static bool is_bit_flag_ ;
    static bool is_bitflags_func(void)
	{
		bool is_bit_flag = true;
		for(size_t i = 0; i < my_count_of(EnumHelper<Enum_T>::all_enums_) ; ++i)
		{
			int n_1 = 0;
			unsigned long long e = static_cast<unsigned long long>(EnumHelper<Enum_T>::all_enums_[i].enum_value);
			for(int k = 0; k < sizeof(e) * 8; ++k )
			{
				if( ((e >> k) & 1) == 1 ) {
					if( ++n_1 > 1) break;
				}
			}
			if( n_1 != 1 ) {
				is_bit_flag = false;
				break;
			}
		}
		return is_bit_flag;
	}
	/// 
	/// @return : no exceptions will be thrown
	template <typename integrate_T> static inline bool generic_is_defined(integrate_T enum_value) throw()
	{
		for(size_t i = 0; i < my_count_of(EnumHelper<Enum_T>::all_enums_) ; i++)
		{
			if( EnumHelper<Enum_T>::all_enums_[i].enum_value == static_cast<Enum_T>(enum_value) )
			{
				return true;
			}
		}
		return false;
	}

	/// @brief: compare whether src is same as dest with case-sensitive
	///         consideration
	/// @note: use this function instead of non-standard
	///        stricmp/strcasecmp
	static bool are_equal(const char * src, const char *dest, bool ignore_case)
	{
		int i = 0;
		if( ignore_case ) {
			for(; src[i] && dest[i] && tolower(src[i]) == tolower(dest[i]) ; ++i) {
			}
		} else {
			for(; src[i] && dest[i] && src[i] == dest[i] ; ++i) {
			}
		}
		return src[i] == 0 &&  dest[i] == 0;
	}

	static inline void set_value(bool & to, Enum_T )
	{
		to = true;
	}
	static inline void set_value(Enum_T & to, Enum_T t)
	{
		to = t;
	}
	static inline void throw_bad_cast_exception_if_Enum_T(Enum_T, const char * enum_str, bool ignore_case)
	{
		std::string str = "[";
		str += enum_str;
		str += "] is not a defined enum item for [";
		str += type_name_;
		str += "] by case-";
		str += (ignore_case)?"insensitive comparison":"sensitive comparison";
		throw std::runtime_error(str);
	}
	static inline void throw_bad_cast_exception_if_Enum_T(bool, const char * /*enum_str*/, bool /*ignore_case*/ )
	{
		; // do nothing other than just return
	}

	// return bool: whether enum_str is defined, return Enum_T: the corresponding enum value
	template <typename return_T> static void from_string(
		return_T & ret, const char * enum_str, bool ignore_case )
	{
		for(size_t i = 0; i < my_count_of(EnumHelper<Enum_T>::all_enums_) ; i++)
		{
			if( are_equal(enum_str, EnumHelper<Enum_T>::all_enums_[i].enum_str, ignore_case) )
			{
				set_value(ret, EnumHelper<Enum_T>::all_enums_[i].enum_value);
				return;
			}
		}
		throw_bad_cast_exception_if_Enum_T(ret, enum_str, ignore_case);
	}

};

#define DEF_ENUM_ONLY_(enum_type, enum_list )  enum enum_type { enum_list };
#define DEF_ENUM_ONLY_WITH_TYPE_(enum_type, underlying_t, enum_list )  enum enum_type : underlying_t { enum_list };

#define REGISTER_ENUM_META_DATA_(ns_cls, enum_type, enum_list)  \
	template<> EnumHelper<ns_cls::enum_type>::GeneralEnumMetaData EnumHelper<ns_cls::enum_type>::all_enums_[] = {\
	enum_list \
};\
	template<> const char * EnumHelper<ns_cls::enum_type>::type_name_ = #enum_type;\
	template<> const int EnumHelper<ns_cls::enum_type>::num_enumerators_ = my_count_of(EnumHelper<ns_cls::enum_type>::all_enums_); \
	template<> bool EnumHelper<ns_cls::enum_type>::is_bit_flag_  = EnumHelper<ns_cls::enum_type>::is_bitflags_func();

#define ENUM_ITEM_VALUE(_12, ns_cls, enum_entry, enum_value) ENUM_ITEM_VALUE_##_12##_(ns_cls, enum_entry, enum_value)
#define ENUM_ITEM(_12, ns_cls, enum_entry) ENUM_ITEM_##_12##_(ns_cls, enum_entry)

// _1 macro used for enum definition
#define ENUM_ITEM_VALUE_1_(ns_cls, enum_entry, enum_value)  enum_entry = enum_value,
#define ENUM_ITEM_1_(ns_cls, enum_entry)  enum_entry ,

// _2 macro used for enum meta data registeration
#define ENUM_ITEM_VALUE_2_(ns_cls, enum_entry, enum_value) {ns_cls::enum_entry, #enum_entry},
#define ENUM_ITEM_2_(ns_cls, enum_entry)  {ns_cls::enum_entry, #enum_entry},

#define DEF_ENUM_1(ns_cls, enum_type, list) DEF_ENUM_ONLY_(enum_type, list(1, ns_cls) )
#define DEF_ENUM_2(ns_cls, enum_type, list) REGISTER_ENUM_META_DATA_(ns_cls, enum_type, list(2, ns_cls) )

#define DEF_ENUM(ns_cls, enum_type, list) \
	DEF_ENUM_ONLY_(enum_type, list(1, ns_cls) ) \
	REGISTER_ENUM_META_DATA_(ns_cls, enum_type, list(2, ns_cls) )

#ifdef _MSC_VER
// MSVC allowed enum : short { Red, Blue };
#define DEF_ENUM_WITH_TYPE(ns_cls, enum_type, underlying_t, list) \
		DEF_ENUM_ONLY_WITH_TYPE_(enum_type, underlying_t, list(1, ns_cls) ) \
		REGISTER_ENUM_META_DATA_(ns_cls, enum_type, list(2, ns_cls) )

#define DEF_ENUM_WITH_TYPE_1(ns_cls, enum_type, underlying_t, list) DEF_ENUM_ONLY_WITH_TYPE_(enum_type, underlying_t, list(1, ns_cls) )
#define DEF_ENUM_WITH_TYPE_2(ns_cls, enum_type, underlying_t, list) REGISTER_ENUM_META_DATA_(ns_cls, enum_type, list(2, ns_cls) )

#endif

#endif // include guard
