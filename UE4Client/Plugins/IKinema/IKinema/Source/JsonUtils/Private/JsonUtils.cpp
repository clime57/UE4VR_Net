// Copyright (C) Microsoft. All rights reserved.

#include "JsonUtilsPrivatePCH.h"

#include "Json.h"
#include "JsonImporter.h"
#include "JsonExporter.h"
#include "JsonWriter.h"

/**
 * JsonUtils implementation
 */

bool FJsonUtils::ImportObjectFromJsonString( UObject* Object, const FString& JsonString, int32 PortFlags )
{
	uint8* ObjectData = (uint8*)Object;
	return FJsonImporter::ImportDataFromJsonString(ObjectData, Object->GetClass(), Object->GetOuter(), JsonString, PortFlags);
}

bool FJsonUtils::ImportObjectFromJson( UObject* Object, const TSharedPtr<class FJsonObject>& JSONObject, int32 PortFlags )
{
	uint8* ObjectData = (uint8*)Object;
	return FJsonImporter::ImportDataFromJson(ObjectData, Object->GetClass(), Object->GetOuter(), JSONObject, PortFlags);
}

bool FJsonUtils::ImportDataFromJsonString( uint8* ObjectData, UStruct* ObjectClass, UObject* Parent, const FString& JsonString, int32 PortFlags )
{
	return FJsonImporter::ImportDataFromJsonString(ObjectData, ObjectClass, Parent, JsonString, PortFlags);
}

bool FJsonUtils::CreateObjectFromJsonString( UObject*& Object, UObject* Parent, const FString& JsonString, int32 PortFlags )
{
	return FJsonImporter::ImportDataFromJsonString( (uint8*&)Object, nullptr, Parent, JsonString, PortFlags);
}

bool FJsonUtils::ExportObjectToJson( const UObject* Object, TSharedPtr<class FJsonValue>& JObjValue, bool bExportClassName, int32 PortFlags )
{
	if (Object == nullptr)
	{
		UE_LOG(LogJsonUtils, Error, TEXT("NULL object data when exporting to JSON"));
		return false;
	}
	return FJsonExporter::ExportDataToJson( (uint8*)Object, Object->GetClass(), Object->GetOuter(), JObjValue, bExportClassName, PortFlags );
}

bool FJsonUtils::ExportObjectToJsonString( const UObject* Object, FString& JSONString, bool bExportClassName, int32 PortFlags )
{
	if (Object == nullptr)
	{
		UE_LOG(LogJsonUtils, Error, TEXT("NULL object data when exporting to JSON"));
		return false;
	}
	return FJsonExporter::ExportDataToJsonString( (uint8*)Object, Object->GetClass(), Object->GetOuter(), JSONString, bExportClassName, PortFlags );
}

bool FJsonUtils::ExportJsonObjectToString( TSharedPtr<FJsonObject>& JObj, FString& JSONString )
{
	JSONString.Empty();
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create( &JSONString );
	return FJsonSerializer::Serialize(JObj.ToSharedRef(), Writer);
}

bool FJsonUtils::ExportDataToJsonString( const uint8* ObjectData, UStruct* ObjectClass, UObject* Parent, FString& JSONString, bool bExportClassName, int32 PortFlags )
{
	return FJsonExporter::ExportDataToJsonString( ObjectData, ObjectClass, Parent, JSONString, bExportClassName, PortFlags );
}