// Copyright (C) Microsoft. All rights reserved.

#pragma once

class JSONUTILS_API FJsonUtils
{
public:

	// Import a UObject from a json string.
	static bool ImportObjectFromJsonString( UObject* Object, const FString& JsonString, int32 PortFlags=PPF_ExportsNotFullyQualified|PPF_Copy );
	// Import a UObject from a json.
	static bool ImportObjectFromJson( UObject* Object, const TSharedPtr<class FJsonObject>& JSONObject, int32 PortFlags=PPF_ExportsNotFullyQualified|PPF_Copy );
	static bool ImportDataFromJsonString( uint8* ObjectData, UStruct* ObjectClass, UObject* Parent, const FString& JsonString, int32 PortFlags=PPF_ExportsNotFullyQualified|PPF_Copy );
	// Create a UObject from a json string.
	static bool CreateObjectFromJsonString( UObject*& Object, UObject* Parent, const FString& JsonString, int32 PortFlags=PPF_ExportsNotFullyQualified|PPF_Copy );

	// Export a UObject to a json string
	static bool ExportObjectToJsonString( const UObject* Object, FString& JSONString, bool bExportClassName, int32 PortFlags=PPF_ExportsNotFullyQualified|PPF_Copy );
	static bool ExportJsonObjectToString( TSharedPtr<FJsonObject>& JObj, FString& JSONString );

	// Export a UObject to a json
	static bool ExportObjectToJson( const UObject* Object, TSharedPtr<class FJsonValue>& JObjValue, bool bExportClassName, int32 PortFlags=PPF_ExportsNotFullyQualified|PPF_Copy );
	static bool ExportDataToJsonString( const uint8* ObjectData, UStruct* ObjectClass, UObject* Parent, FString& JSONString, bool bExportClassName, int32 PortFlags=PPF_ExportsNotFullyQualified|PPF_Copy );
};
