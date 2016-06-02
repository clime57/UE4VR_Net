// Copyright (C) Microsoft. All rights reserved.

#pragma once

class FJsonValue;

// Utility to Export a UObject to JSON.
class FJsonExporter
{
public:
	
	static bool ExportDataToJsonString( const uint8* ObjectData, UStruct* ObjectClass, UObject* Parent, FString& JSONString, bool bExportClassName, int32 PortFlags);	
	static bool ExportDataToJson( const uint8* ObjectData, UStruct* ObjectClass, UObject* Parent, TSharedPtr<FJsonValue>& JObjValue, bool bExportClassName, int32 PortFlags );

private:

	// Should never create an instance of this class
	FJsonExporter() {}

	static void Export( TSharedPtr<FJsonValue>& JObjValue, UStruct* ObjectClass, const uint8* ObjectData, UObject* Parent, int32 PortFlags, bool bExportClassName );
	
	static void ExportProperty( TSharedPtr<FJsonValue>& JObjValue, UProperty* Prop, const uint8* PropertyValue, const uint8* DefaultValue, UObject* Parent, int32 PortFlags, int32 ArrayIndex, UObject* ExportRootScope = NULL );

	static void ExportPropertyBool( TSharedPtr<FJsonValue>& JObjValue, UBoolProperty* Prop, const uint8* PropertyValue, const uint8* DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope=NULL );
	static void ExportPropertyByte( TSharedPtr<FJsonValue>& JObjValue, UByteProperty* Prop, const uint8* PropertyValue, const uint8* DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope=NULL );	
	static void ExportPropertyAssetReference( TSharedPtr<FJsonValue>& JObjValue, UObjectPropertyBase* Prop, const uint8* PropertyValue, const uint8* DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope=NULL );
	static void ExportPropertySubObject( TSharedPtr<FJsonValue>& JObjValue, UObjectPropertyBase* Prop, const uint8* PropertyValue, const uint8* DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope=NULL );
	static void ExportPropertyStruct( TSharedPtr<FJsonValue>& JObjValue, UStructProperty* Prop, const uint8* PropertyValue, const uint8* DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope=NULL );
	static void ExportPropertyArray( TSharedPtr<FJsonValue>& JObjValue, UArrayProperty* Prop, const uint8* PropertyValue, const uint8* DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope=NULL );
	static void ExportPropertyString( TSharedPtr<FJsonValue>& JObjValue, UProperty* Prop, const uint8* PropertyValue, const uint8* DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope=NULL );

};
