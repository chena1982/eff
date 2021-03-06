/* ----------------------------------------------------------------------------
** Copyright (c) 2016 Austin Brunkhorst, All Rights Reserved.
**
** Type.cpp
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "Type.h"
#include "TypeCreator.h"

#include "Variant.h"
#include "Enum.h"

#include "Constructor.h"
#include "Destructor.h"
#include "Field.h"
#include "Method.h"
#include "Function.h"

#include "MetaManager.h"
#include "RuntimeMetaProperties.h"

#include "ReflectionDatabase.h"

#include "Common/Logging.h"

namespace ursine
{
    namespace meta
    {
        namespace
        {
            // make sure we always have a reference to the gDatabase
            #define gDatabase ReflectionDatabase::Instance( )
        }

        Type::Type(void)
            : m_id( InvalidTypeID )
            , m_isArray( false ) { }

        ///////////////////////////////////////////////////////////////////////

        Type::Type(const Type &rhs)
            : m_id( rhs.m_id )
            , m_isArray( rhs.m_isArray ) { }

        ///////////////////////////////////////////////////////////////////////

        Type::Type(TypeID id, bool isArray)
            : m_id( id )
            , m_isArray( isArray ) { }

        ///////////////////////////////////////////////////////////////////////

        Type::operator bool(void) const
        {
            return m_id != InvalidTypeID;
        }

        ///////////////////////////////////////////////////////////////////////

        Type &Type::operator=(const Type &rhs)
        {
            m_id = rhs.m_id;
            m_isArray = rhs.m_isArray;

            return *this;
        }

        ///////////////////////////////////////////////////////////////////////

        bool Type::operator<(const Type &rhs) const
        {
            return m_id < rhs.m_id;
        }

        ///////////////////////////////////////////////////////////////////////

        bool Type::operator>(const Type &rhs) const
        {
            return m_id > rhs.m_id;
        }

        ///////////////////////////////////////////////////////////////////////

        bool Type::operator<=(const Type &rhs) const
        {
            return m_id <= rhs.m_id;
        }

        ///////////////////////////////////////////////////////////////////////

        bool Type::operator>=(const Type &rhs) const
        {
            return m_id >= rhs.m_id;
        }

        ///////////////////////////////////////////////////////////////////////

        bool Type::operator==(const Type &rhs) const
        {
            return m_id == rhs.m_id;
        }

        ///////////////////////////////////////////////////////////////////////

        bool Type::operator!=(const Type &rhs) const
        {
            return m_id != rhs.m_id;
        }

        ///////////////////////////////////////////////////////////////////////

        const Type &Type::Invalid(void)
        {
            static const Type invalid { InvalidTypeID };

            return invalid;
        }

        ///////////////////////////////////////////////////////////////////////

        TypeID Type::GetID(void) const
        {
            return m_id;
        }

        ///////////////////////////////////////////////////////////////////////

        Type::List Type::GetTypes(void)
        {
            auto count = gDatabase.types.size( );

            List types;

            // skip the first one, as it's reserved for unknown
            for (TypeID i = 1; i < count; ++i)
                types.emplace_back( i );

            return types;
        }

        ///////////////////////////////////////////////////////////////////////

        std::vector<Global> Type::GetGlobals(void)
        {
            std::vector<Global> globals;

            for (auto &global : gDatabase.globals)
                globals.emplace_back( global.second );

            return globals;
        }

        ///////////////////////////////////////////////////////////////////////

        const Global &Type::GetGlobal(const std::string &name)
        {
            return gDatabase.globals[ name ];
        }

        ///////////////////////////////////////////////////////////////////////

        std::vector<Function> Type::GetGlobalFunctions(void)
        {
            std::vector<Function> functions;

            for (auto &overload : gDatabase.globalFunctions)
            {
                for (auto &function : overload.second)
                {
                    functions.emplace_back( function.second );
                }
            }

            return functions;
        }

        ///////////////////////////////////////////////////////////////////////

        const Function &Type::GetGlobalFunction(const std::string &name)
        {
            return gDatabase.GetGlobalFunction( name );
        }

        ///////////////////////////////////////////////////////////////////////

        const Function &Type::GetGlobalFunction(
            const std::string &name, 
            const InvokableSignature &signature
        )
        {
            return gDatabase.GetGlobalFunction( name, signature );
        }

        ///////////////////////////////////////////////////////////////////////

        Type Type::GetFromName(const std::string &name)
        {
            auto search = gDatabase.ids.find( name );

            if (search == gDatabase.ids.end( ))
                return Invalid( );

            return { search->second };
        }

        ///////////////////////////////////////////////////////////////////////

        bool Type::ListsEqual(const List &a, const List &b)
        {
            if (a.size( ) != b.size( ))
                return false;

            auto equal = true;

            for (size_t i = 0u, size = a.size( ); equal && i < size; ++i)
                equal = (a[ i ] == b[ i ]);

            return equal;
        }

        ///////////////////////////////////////////////////////////////////////

        bool Type::IsValid(void) const
        {
            return m_id != InvalidTypeID;
        }

        ///////////////////////////////////////////////////////////////////////

        bool Type::IsPrimitive(void) const
        {
            return gDatabase.types[ m_id ].isPrimitive;
        }

        ///////////////////////////////////////////////////////////////////////

        bool Type::IsFloatingPoint(void) const
        {
            return gDatabase.types[ m_id ].isFloatingPoint;
        }

        ///////////////////////////////////////////////////////////////////////

        bool Type::IsSigned(void) const
        {
            return gDatabase.types[ m_id ].isSigned;
        }

        ///////////////////////////////////////////////////////////////////////

        bool Type::IsEnum(void) const
        {
            return gDatabase.types[ m_id ].isEnum;
        }

        ///////////////////////////////////////////////////////////////////////

        bool Type::IsPointer(void) const
        {
            return gDatabase.types[ m_id ].isPointer;
        }

        ///////////////////////////////////////////////////////////////////////

        bool Type::IsClass(void) const
        {
            return gDatabase.types[ m_id ].isClass;
        }

        ///////////////////////////////////////////////////////////////////////

        bool Type::IsArray(void) const
        {
            return m_isArray;
        }

        ///////////////////////////////////////////////////////////////////////

        std::string Type::GetName(void) const
        {
            auto &name = gDatabase.types[ m_id ].name;

            if (IsArray( ))
                return "Array<" + name + ">";

            return name;
        }

        const MetaManager &Type::GetMeta(void) const
        {
            return gDatabase.types[ m_id ].meta;
        }

        ///////////////////////////////////////////////////////////////////////

        void Type::Destroy(Variant &instance) const
        {
            auto &destructor = gDatabase.types[ m_id ].destructor;

            if (destructor.IsValid( ))
                destructor.Invoke( instance );
        }

        ///////////////////////////////////////////////////////////////////////

        Type Type::GetDecayedType(void) const
        {
            UAssert( false, "Type::GetDecayedType() not implemented." );

            // @@@TODO: convert to non pointer/const pointer type
            return Type( );
        }

        ///////////////////////////////////////////////////////////////////////

        Type Type::GetArrayType(void) const
        {
            return Type( m_id, false );
        }

        ///////////////////////////////////////////////////////////////////////

        const Enum &Type::GetEnum(void) const
        {
            return gDatabase.types[ m_id ].enumeration;
        }

        ///////////////////////////////////////////////////////////////////////

        bool Type::DerivesFrom(const Type &other) const
        {
            auto &baseClasses = gDatabase.types[ m_id ].baseClasses;

            return baseClasses.find( other ) != baseClasses.end( );
        }

        ///////////////////////////////////////////////////////////////////////

        const Type::Set &Type::GetBaseClasses(void) const
        {
            return gDatabase.types[ m_id ].baseClasses;
        }

        ///////////////////////////////////////////////////////////////////////

        const Type::Set &Type::GetDerivedClasses(void) const
        {
            return gDatabase.types[ m_id ].derivedClasses;
        }

        ///////////////////////////////////////////////////////////////////////

        std::vector<Constructor> Type::GetConstructors(void) const
        {
            auto &handle = gDatabase.types[ m_id ].constructors;

            std::vector<Constructor> constructors;

            for (auto &constructor : handle)
                constructors.emplace_back( constructor.second );

            return constructors;
        }

        ///////////////////////////////////////////////////////////////////////

        std::vector<Constructor> Type::GetDynamicConstructors(void) const
        {
            auto &handle = gDatabase.types[ m_id ].dynamicConstructors;

            std::vector<Constructor> constructors;

            for (auto &constructor : handle)
                constructors.emplace_back( constructor.second );

            return constructors;
        }

        ///////////////////////////////////////////////////////////////////////

        const Constructor &Type::GetConstructor(
            const InvokableSignature &signature
        ) const
        {
            return gDatabase.types[ m_id ].GetConstructor( signature );
        }

        ///////////////////////////////////////////////////////////////////////

        const Constructor &Type::GetDynamicConstructor(
            const InvokableSignature &signature
        ) const
        {
            return gDatabase.types[ m_id ].GetDynamicConstructor( signature );
        }

        ///////////////////////////////////////////////////////////////////////

        const Constructor &Type::GetArrayConstructor(void) const
        {
            return gDatabase.types[ m_id ].arrayConstructor;
        }

        ///////////////////////////////////////////////////////////////////////

        const Destructor &Type::GetDestructor(void) const
        {
            return gDatabase.types[ m_id ].destructor;
        }

        ///////////////////////////////////////////////////////////////////////

        std::vector<Method> Type::GetMethods(void) const
        {
            std::vector<Method> methods;

            auto &handle = gDatabase.types[ m_id ].methods;

            for (auto &overload : handle)
            {
                for (auto &method : overload.second)
                {
                    methods.emplace_back( method.second );
                }
            }

            return methods;
        }

        ///////////////////////////////////////////////////////////////////////

        const Method &Type::GetMethod(const std::string &name) const
        {
            return gDatabase.types[ m_id ].GetMethod( name );
        }

        ///////////////////////////////////////////////////////////////////////

        const Method &Type::GetMethod(
            const std::string &name, 
            const InvokableSignature &signature
        ) const
        {
            return gDatabase.types[ m_id ].GetMethod( name, signature );
        }

        ///////////////////////////////////////////////////////////////////////

        std::vector<Function> Type::GetStaticMethods(void) const
        {
            std::vector<Function> methods;

            auto &handle = gDatabase.types[ m_id ].staticMethods;

            for (auto &overload : handle)
            {
                for (auto &method : overload.second)
                {
                    methods.emplace_back( method.second );
                }
            }

            return methods;
        }

        ///////////////////////////////////////////////////////////////////////

        const Function &Type::GetStaticMethod(const std::string &name) const
        {
            return gDatabase.types[ m_id ].GetStaticMethod( name );
        }

        ///////////////////////////////////////////////////////////////////////

        const Function &Type::GetStaticMethod(
            const std::string &name, 
            const InvokableSignature &signature
        ) const
        {
            return gDatabase.types[ m_id ].GetStaticMethod( name, signature );
        }

        ///////////////////////////////////////////////////////////////////////

        const std::vector<Field> &Type::GetFields(void) const
        {
            // @@@TODO: recursively get base class fields

            return gDatabase.types[ m_id ].fields;
        }

        ///////////////////////////////////////////////////////////////////////

        const Field &Type::GetField(const std::string &name) const
        {
            return gDatabase.types[ m_id ].GetField( name );
        }

        ///////////////////////////////////////////////////////////////////////

        std::vector<Global> Type::GetStaticFields(void) const
        {
            return gDatabase.types[ m_id ].staticFields;
        }

        ///////////////////////////////////////////////////////////////////////

        const Global &Type::GetStaticField(const std::string &name) const
        {
            return gDatabase.types[ m_id ].GetStaticField( name );
        }

        ///////////////////////////////////////////////////////////////////////

        Json11 Type::SerializeJson(const Variant &instance, bool invokeHook) const
        {
            UAssert(
                *this == instance.GetType( ),
                "Serializing incompatible variant instance.\n"
                "Got '%s', expected '%s'",
                instance.GetType( ).GetName( ).c_str( ),
                GetName( ).c_str( )
            );

            if (m_isArray)
            {
                Json11::array array;

                auto wrapper = instance.GetArray( );
                auto size = wrapper.Size( );

                for (size_t i = 0; i < size; ++i)
                {
                    auto value = wrapper.GetValue( i );

                    array.emplace_back( 
                        value.GetType( ).SerializeJson( value, invokeHook ) 
                    );
                }

                return array;
            }

            if (*this == typeof( bool ))
            {
                return { instance.ToBool( ) };
            }

            auto &meta = GetMeta( );
            auto isEnum = IsEnum( );

            // number, or non-associative enum
            if (IsPrimitive( ) || meta.GetProperty<SerializeAsNumber>( ))
            {
                if (IsFloatingPoint( ) || !IsSigned( ))
                    return { instance.ToDouble( ) };
 
                return { instance.ToInt( ) };
            }

            // associative enum value
            if (isEnum)
            {
                return GetEnum( ).GetKey( instance );
            }

            if (*this == typeof( std::string ))
            {
                return { instance.ToString( ) };
            }
            
            Json11::object object { };

            auto &fields = gDatabase.types[ m_id ].fields;

            for (auto &field : fields)
            {
                auto value = field.GetValue( instance );

                auto json = value.SerializeJson( );

                value.m_base->OnSerialize( const_cast<Json11::object&>( json.object_items( ) ) );

                object[ field.GetName( ) ] = json;
            }

            if (invokeHook)
                instance.m_base->OnSerialize( object );

            return object;
        }

        ///////////////////////////////////////////////////////////////////////

        Json11 Type::SerializeJson(const Variant &instance, SerializationGetterOverride getterOverride, bool invokeHook) const
        {
            UAssert(
                *this == instance.GetType( ),
                "Serializing incompatible variant instance.\n"
                "Got '%s', expected '%s'",
                instance.GetType( ).GetName( ).c_str( ),
                GetName( ).c_str( )
            );

            if (IsArray( ))
            {
                Json11::array array;

                auto wrapper = instance.GetArray( );
                auto size = wrapper.Size( );

                for (size_t i = 0; i < size; ++i)
                {
                    auto value = wrapper.GetValue( i );

                    array.emplace_back( 
                        value.GetType( ).SerializeJson( value, invokeHook )
                    );
                }

                return array;
            }

            if (*this == typeof( bool ))
            {
                return { instance.ToBool( ) };
            }

            auto &meta = GetMeta( );
            auto isEnum = IsEnum( );

            // number, or non-associative enum
            if (IsPrimitive( ) || meta.GetProperty<SerializeAsNumber>( ))
            {
                if (IsFloatingPoint( ) || !IsSigned( ))
                    return { instance.ToDouble( ) };
 
                return { instance.ToInt( ) };
            }

            // associative enum value
            if (isEnum)
            {
                return GetEnum( ).GetKey( instance );
            }

            if (*this == typeof( std::string ))
            {
                return { instance.ToString( ) };
            }
            
            Json11::object object { };

            auto &fields = gDatabase.types[ m_id ].fields;

            for (auto &field : fields)
            {
                auto value = getterOverride( instance, field );

                auto json = value.SerializeJson( );

                value.m_base->OnSerialize( const_cast<Json11::object&>( json.object_items( ) ) );

                object[ field.GetName( ) ] = json;
            }

            if (invokeHook)
                instance.m_base->OnSerialize( object );

            return object;
        }

        ///////////////////////////////////////////////////////////////////////

        Variant Type::DeserializeJson(const Json11 &value) const
        {
            auto &ctor = GetConstructor( );

            UAssert( ctor.IsValid( ),
                "Serialization requires a default constructor.\nWith type '%s'.",
                GetName( ).c_str( )
            );

            return DeserializeJson( value, ctor );
        }

        ///////////////////////////////////////////////////////////////////////

        Variant Type::DeserializeJson(const Json11 &value, const Constructor &ctor) const
        {
            // array types get special care
            if (IsArray( ))
            {
                auto nonArrayType = GetArrayType( );
                auto arrayCtor = GetArrayConstructor( );

                UAssert( arrayCtor.IsValid( ),
                    "Type '%s' does not have an array constructor.\n"
                    "Makes sure it is enabled with the meta property 'EnableArrayType'.",
                    nonArrayType.GetName( ).c_str( )
                );

                auto instance = arrayCtor.Invoke( );
                auto wrapper = instance.GetArray( );

                size_t i = 0;

                for (auto &item : value.array_items( ))
                {
                    wrapper.Insert( 
                        i++, 
                        nonArrayType.DeserializeJson( item, ctor ) 
                    );
                }

                return instance;
            }
            // we have to handle all primitive types explicitly
            else if (IsPrimitive( ))
            {
                if (*this == typeof( int ))
                    return { value.int_value( ) };
                else if (*this == typeof( unsigned int ))
                    return { static_cast<unsigned int>( value.number_value( ) ) };
                else if (*this == typeof( bool ))
                    return { value.bool_value( ) };
                else if (*this == typeof( float ))
                    return { static_cast<float>( value.number_value( ) ) };
                else if (*this == typeof( double ))
                    return { value.number_value( ) };
            }
            else if (IsEnum( ))
            {
                // number literal
                if (value.is_number( ))
                    return { value.int_value( ) };

                // associative value
                auto enumValue = GetEnum( ).GetValue( value.string_value( ) );

                // make sure we can find the key
                if (enumValue.IsValid( ))
                    return enumValue;
                
                // use the default value as we couldn't find the key
                return TypeCreator::Create( *this );
            }
            else if (*this == typeof( std::string ))
            {
                return { value.string_value( ) };
            }

            // @@@TODO: forward arguments to constructor
            auto instance = ctor.Invoke( );

            DeserializeJson( instance, value );

            return instance;
        }

        ///////////////////////////////////////////////////////////////////////

        void Type::DeserializeJson(Variant &instance, const Json11 &value) const
        {
            auto &fields = gDatabase.types[ m_id ].fields;

            for (auto &field : fields)
            {
                auto fieldType = field.GetType( );

                UAssert( fieldType.IsValid( ),
                    "Unknown type for field '%s' in base type '%s'. Is this type reflected?",
                    fieldType.GetName( ).c_str( ),
                    GetName( ).c_str( )
                );

                auto &fieldData = value[ field.GetName( ) ];

                if (!fieldData.is_null( ))
                {
                    auto fieldValue = fieldType.DeserializeJson( fieldData );

                    fieldValue.m_base->OnDeserialize( fieldData );

                    field.SetValue( instance, fieldValue );
                }
            }

            instance.m_base->OnDeserialize( value );
        }

        ///////////////////////////////////////////////////////////////////////

        void Type::SerializeYaml(YAML::Emitter & output, const Variant &instance, bool invokeHook) const
        {
            UAssert(
                *this == instance.GetType(),
                "Serializing incompatible variant instance.\n"
                "Got '%s', expected '%s'",
                instance.GetType().GetName().c_str(),
                GetName().c_str()
            );

            output << YAML::Key << GetName();
            output << YAML::Value;

            if (m_isArray)
            {
                auto wrapper = instance.GetArray();
                auto size = wrapper.Size();

                output << YAML::Flow;
                output << YAML::BeginSeq;

                for (size_t i = 0; i < size; ++i)
                {
                    auto value = wrapper.GetValue(i);

                    output << value;
                }

                output << YAML::EndSeq;
            }






            if (*this == typeof(bool))
            {
                output << instance.ToBool();
            }

            auto &meta = GetMeta();
            auto isEnum = IsEnum();

            // number, or non-associative enum
            if (IsPrimitive() || meta.GetProperty<SerializeAsNumber>())
            {
                if (IsFloatingPoint() || !IsSigned())
                {
                    output << instance.ToDouble();
                }
                else
                {
                    output << instance.ToInt();
                }
            }

            // associative enum value
            if (isEnum)
            {
                output << GetEnum().GetKey(instance);
            }

            if (*this == typeof(std::string))
            {
                output << instance.ToString();
            }


            output << YAML::BeginMap;

            auto &fields = gDatabase.types[m_id].fields;

            for (auto &field : fields)
            {
                auto value = field.GetValue(instance);

                YAML::Emitter fieldResult;

                value.m_base->OnSerialize(fieldResult);

                //result << fieldResult;

                output << YAML::Key << fieldResult.c_str();
                output << YAML::Value << fieldResult.c_str();
            }

            output << YAML::EndMap;

            //if (invokeHook)
            //    instance.m_base->OnSerialize(object);

        }

        ///////////////////////////////////////////////////////////////////////

        void Type::SerializeYaml(YAML::Emitter & output, const Variant &instance, SerializationGetterOverride getterOverride, bool invokeHook) const
        {
            UAssert(
                *this == instance.GetType(),
                "Serializing incompatible variant instance.\n"
                "Got '%s', expected '%s'",
                instance.GetType().GetName().c_str(),
                GetName().c_str()
            );

            output << YAML::Key << GetName();
            output << YAML::Value;

            if (IsArray())
            {
                auto wrapper = instance.GetArray();
                auto size = wrapper.Size();

                output << YAML::Flow;
                output << YAML::BeginSeq;

                for (size_t i = 0; i < size; ++i)
                {
                    auto value = wrapper.GetValue(i);

                    output << value;
                }

                output << YAML::EndSeq;
            }

            if (*this == typeof(bool))
            {
                output << instance.ToBool();
            }

            auto &meta = GetMeta();
            auto isEnum = IsEnum();

            // number, or non-associative enum
            if (IsPrimitive() || meta.GetProperty<SerializeAsNumber>())
            {
                if (IsFloatingPoint() || !IsSigned())
                {
                    output << instance.ToDouble();
                }
                else
                {
                    output << instance.ToInt();
                }
            }

            // associative enum value
            if (isEnum)
            {
                output << GetEnum().GetKey(instance);
            }

            if (*this == typeof(std::string))
            {
                output << instance.ToString();
            }

            output << YAML::BeginMap;

            auto &fields = gDatabase.types[m_id].fields;

            for (auto &field : fields)
            {
                auto value = getterOverride(instance, field);

                YAML::Emitter fieldResult;

                value.m_base->OnSerialize(fieldResult);

                output << YAML::Key << fieldResult.c_str();
                output << YAML::Value << fieldResult.c_str();
            }

            output << YAML::EndMap;

            //if (invokeHook)
            //    instance.m_base->OnSerialize(object);

        }

        ///////////////////////////////////////////////////////////////////////

        Variant Type::DeserializeYaml(const YAML::Node &value) const
        {
            auto &ctor = GetConstructor();

            UAssert(ctor.IsValid(),
                "Serialization requires a default constructor.\nWith type '%s'.",
                GetName().c_str()
            );

            return DeserializeYaml(value, ctor);
        }

        ///////////////////////////////////////////////////////////////////////

        Variant Type::DeserializeYaml(const YAML::Node &value, const Constructor &ctor) const
        {
            // array types get special care
            if (IsArray())
            {
                auto nonArrayType = GetArrayType();
                auto arrayCtor = GetArrayConstructor();

                UAssert(arrayCtor.IsValid(),
                    "Type '%s' does not have an array constructor.\n"
                    "Makes sure it is enabled with the meta property 'EnableArrayType'.",
                    nonArrayType.GetName().c_str()
                );

                auto instance = arrayCtor.Invoke();
                auto wrapper = instance.GetArray();


                for (size_t i = 0; i < value.size(); i++)
                {
                    wrapper.Insert(i, nonArrayType.DeserializeYaml(value[nonArrayType.GetName()], ctor));
                }

                return instance;
            }
            // we have to handle all primitive types explicitly
            else if (IsPrimitive())
            {
                if (*this == typeof(int))
                    return { value.as<int>() };
                else if (*this == typeof(unsigned int))
                    return { value.as<unsigned int>() };
                else if (*this == typeof(bool))
                    return { value.as<bool>() };
                else if (*this == typeof(float))
                    return { value.as<float>() };
                else if (*this == typeof(double))
                    return { value.as<double>() };
            }
            else if (IsEnum())
            {
                // number literal
                if (value.IsScalar())
                    return { value.as<int>() };

                // associative value
                auto enumValue = GetEnum().GetValue(value.as<std::string>());

                // make sure we can find the key
                if (enumValue.IsValid())
                    return enumValue;

                // use the default value as we couldn't find the key
                return TypeCreator::Create(*this);
            }
            else if (*this == typeof(std::string))
            {
                return { value.as<std::string>() };
            }

            // @@@TODO: forward arguments to constructor
            auto instance = ctor.Invoke();

            DeserializeYaml(instance, value);

            return instance;
        }

        ///////////////////////////////////////////////////////////////////////

        void Type::DeserializeYaml(Variant &instance, const YAML::Node &value) const
        {
            auto &fields = gDatabase.types[m_id].fields;

            for (auto &field : fields)
            {
                auto fieldType = field.GetType();

                UAssert(fieldType.IsValid(),
                    "Unknown type for field '%s' in base type '%s'. Is this type reflected?",
                    fieldType.GetName().c_str(),
                    GetName().c_str()
                );

                auto &fieldData = value[field.GetName()];

                if (!fieldData.IsNull())
                {
                    auto fieldValue = fieldType.DeserializeYaml(fieldData);

                    fieldValue.m_base->OnDeserialize(fieldData);

                    field.SetValue(instance, fieldValue);
                }
            }

            instance.m_base->OnDeserialize(value);
        }
    }
}
