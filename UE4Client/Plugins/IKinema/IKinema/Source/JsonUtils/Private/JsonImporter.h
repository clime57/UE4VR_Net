// Copyright (C) Microsoft. All rights reserved.

#pragma once

// Utility to import a UObject from JSON.
class FJsonImporter
{
public:

	static bool ImportDataFromJsonString( uint8*& ObjectData, UStruct* ObjectClass, UObject* Parent, const FString& JSONString, int32 PortFlags );
	static bool ImportDataFromJson( uint8*& ObjectData, UStruct* ObjectClass, UObject* Parent, const TSharedPtr<class FJsonObject>& JObjValue, int32 PortFlags );

private:

	// Should never create an instance of this class
	FJsonImporter() {}

	static bool Import( uint8* ObjectData, UStruct* ObjectClass, const TSharedPtr<class FJsonObject>& JObj, UObject* Parent, int32 PortFlags, FOutputDevice* ErrorText );

	static bool ImportProperty( const TSharedPtr<class FJsonValue>& JObjValue, UProperty* Prop, uint8* PropertyValue, UObject* Parent, int32 PortFlags, int32 ArrayIndex, FOutputDevice* ErrorText );

	static bool ImportPropertyByte( const TSharedPtr<class FJsonValue>& JObjValue, UByteProperty* Prop, uint8* Data, int32 PortFlags, UObject* Parent, FOutputDevice* ErrorText );
	static bool ImportPropertyBool( const TSharedPtr<class FJsonValue>& JObjValue, UBoolProperty* Prop, uint8* Data, int32 PortFlags, UObject* Parent, FOutputDevice* ErrorText );
	static bool ImportPropertyAssetReference( const TSharedPtr<class FJsonValue>& JObjValue, UObjectPropertyBase* Prop, uint8* Data, int32 PortFlags, UObject* Parent, FOutputDevice* ErrorText );
	static bool ImportPropertySubObject( const TSharedPtr<class FJsonValue>& JObjValue, UObjectPropertyBase* Prop, uint8* Data, int32 PortFlags, UObject* Parent, FOutputDevice* ErrorText );
	static bool ImportPropertyStruct( const TSharedPtr<class FJsonValue>& JObjValue, UStructProperty* Prop, uint8* Data, int32 PortFlags, UObject* Parent, FOutputDevice* ErrorText );
	static bool ImportPropertyArray( const TSharedPtr<class FJsonValue>& JObjValue, UArrayProperty* Prop, uint8* Data, int32 PortFlags, UObject* Parent, FOutputDevice* ErrorText );
	static bool ImportPropertyString( const TSharedPtr<class FJsonValue>& JObjValue, UProperty* Prop, uint8* Data, int32 PortFlags, UObject* Parent, FOutputDevice* ErrorText );
};
