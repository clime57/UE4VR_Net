// Copyright (C) Microsoft. All rights reserved.

// LIST_SIMPLE_PROPERTY_TYPES is undef in each cpp, do not include in include guard
//////////////////////////////////////////////////////////////////////////
// Begin master list of number properties.

#define LIST_SIMPLE_PROPERTY_TYPES(Op) \
	Op(int16, UInt16Property ) \
	Op(int32, UIntProperty) \
	Op(int64, UInt64Property) \
	Op(uint16, UUInt16Property) \
	Op(uint32, UUInt32Property) \
	Op(uint64, UUInt64Property) \
	Op(float, UFloatProperty) \

// End master list of number properties.
//////////////////////////////////////////////////////////////////////////

#ifndef JSONUTILCOMMON_H
#define JSONUTILCOMMON_H

template <typename T>
struct FJsonNumberPropertyTraits;

#define MAP_PROPERTY_TYPE(CppType, UEType) \
template <> \
struct FJsonNumberPropertyTraits<CppType> \
{ \
	typedef class UEType PropertyType; \
};

// define traits types
LIST_SIMPLE_PROPERTY_TYPES( MAP_PROPERTY_TYPE )	
#undef MAP_PROPERTY_TYPE

#endif /* JSONUTILCOMMON_H */