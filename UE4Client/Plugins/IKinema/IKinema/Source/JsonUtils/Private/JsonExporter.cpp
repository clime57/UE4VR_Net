// Copyright (C) Microsoft. All rights reserved.

#include "JsonUtilsPrivatePCH.h"

#include "JsonUtilCommon.h"
#include "JsonExporter.h"
#include "Json.h"

#define LOC_NAMESPACE TEXT("JsonUtils")

// Utilities to export number properties from a JSON object to the actual property.

class FJsonFromNumberPropertyBase
{
public:

	virtual bool AcceptsProperty( UProperty* Prop ) const = 0;

	virtual void ExportValue( TSharedPtr<FJsonValue>& JObjValue, const uint8* PropertyValue ) const = 0;	
};

// Template implementation of the above.
template <typename T>
class FJsonFromNumberProperty : public FJsonFromNumberPropertyBase
{
protected:

	typedef typename typename FJsonNumberPropertyTraits<T>::PropertyType PropertyType;

	bool AcceptsProperty( UProperty* Prop ) const
	{
		return Prop->IsA( PropertyType::StaticClass() );
	}

	virtual void ExportValue( TSharedPtr<FJsonValue>& JObjValue, const uint8* PropertyValue ) const
	{
		const T Value = *(const T*)PropertyValue;
		JObjValue = MakeShareable(new FJsonValueNumber( Value ));
	}
};

bool FJsonExporter::ExportDataToJson( const uint8* ObjectData, UStruct* ObjectClass, UObject* Parent, TSharedPtr<FJsonValue>& JObjValue, bool bExportClassName, int32 PortFlags )
{
	if (ObjectData == nullptr)
	{
		UE_LOG(LogJsonUtils, Error, TEXT("NULL object data when exporting to JSON"));
		return false;
	}

	if (ObjectClass == nullptr)
	{
		UE_LOG(LogJsonUtils, Error, TEXT("NULL ObjectClass data when exporting to JSON"));
		return false;
	}
	
	FJsonExporter::Export( JObjValue, ObjectClass, ObjectData, Parent, PortFlags, bExportClassName );
	if ( !JObjValue.IsValid() )
	{
		UE_LOG(LogJsonUtils, Error, TEXT("error exporting to JSON"));
		return false;
	}	
	return true;
}


bool FJsonExporter::ExportDataToJsonString( const uint8* ObjectData, UStruct* ObjectClass, UObject* Parent, FString& JSONString, bool bExportClassName, int32 PortFlags )
{
	if (ObjectData == nullptr)
	{
		UE_LOG(LogJsonUtils, Error, TEXT("NULL object data when exporting to JSON"));
		return false;
	}

	if (ObjectClass == nullptr)
	{
		UE_LOG(LogJsonUtils, Error, TEXT("NULL ObjectClass data when exporting to JSON"));
		return false;
	}

	TSharedPtr<FJsonValue> JObjValue;
	if ( FJsonExporter::ExportDataToJson( ObjectData, ObjectClass, Parent, JObjValue, bExportClassName, PortFlags ) )
	{
		TSharedPtr<FJsonObject> JObj = JObjValue->AsObject();
		TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create( &JSONString );
		if (!FJsonSerializer::Serialize( JObj.ToSharedRef(), Writer ))
		{
			UE_LOG(LogJsonUtils, Error, TEXT("Error serializing json object to string"));
			return false;
		}
	}		
	return true;
}

void FJsonExporter::Export( TSharedPtr<FJsonValue>& JObjValue, UStruct* ObjectClass, const uint8* ObjectData, UObject* Parent, int32 PortFlags, bool bExportClassName )
{
	TSharedPtr<FJsonObject> JObj = MakeShareable(new FJsonObject());
	check(ObjectClass != nullptr);

	if ( bExportClassName )
	{
		// Add Class Property before other properties for readability		
		JObj->Values.Add( TEXT("Class"), MakeShareable(new FJsonValueString( ObjectClass->GetName() )));		
	}	
	// Export property one by one, See UnrealExporter::ExportProperties
	for( UProperty* Property = ObjectClass->PropertyLink; Property; Property = Property->PropertyLinkNext )
	{
		if (Property->ShouldPort(PortFlags))
		{
			UObjectPropertyBase* ExportObjectProp = (Property->PropertyFlags & CPF_ExportObject) != 0 ? Cast<UObjectPropertyBase>(Property) : NULL;
			
			/*if ( (PortFlags & PPF_LocalizedOnly) != 0 && !Property->IsLocalized() )
			{
				continue;
			}*/

			TSharedPtr<FJsonValue> JObjPropValue = nullptr;
			ExportProperty(
				JObjPropValue,
				Property,
				Property->ContainerPtrToValuePtr<uint8>(ObjectData),
				nullptr,
				Parent,
				PortFlags,
				-1,
				NULL
				);
			if ( JObjPropValue.IsValid() )
			{					
				JObj->Values.Add( Property->GetName(), JObjPropValue );	
			}						
		}
	}
	JObjValue = MakeShareable(new FJsonValueObject(JObj));
}

void FJsonExporter::ExportProperty( TSharedPtr<FJsonValue>& JObjValue, UProperty* Prop, const uint8* PropertyValue, const uint8* DefaultValue, UObject* Parent, int32 PortFlags, int32 ArrayIndex, UObject* ExportRootScope )
{
	// Static list of number converters.

#define STATIC_JSON_CONVERTER(CppType, UEType ) \
	static const FJsonFromNumberProperty<CppType> sJSONFromNumber_##UEType;

	LIST_SIMPLE_PROPERTY_TYPES( STATIC_JSON_CONVERTER )

#undef STATIC_JSON_CONVERTER

#define STATIC_JSON_CONVERTER_ELEMENT(CppType, UEType ) &sJSONFromNumber_##UEType,

	// A corresponding static list of converter base class pointers
	static const FJsonFromNumberPropertyBase* sJSONFromNumberConverters[] =
	{
		LIST_SIMPLE_PROPERTY_TYPES(STATIC_JSON_CONVERTER_ELEMENT)
	};

#undef STATIC_JSON_CONVERTER_ELEMENT

	// Handle number properties.
	static const uint32 sNumConverters = sizeof(sJSONFromNumberConverters)/sizeof(sJSONFromNumberConverters[0]);
	bool handled = false;
	for ( uint32 i=0 ; i < sNumConverters ; ++i )
	{
		const FJsonFromNumberPropertyBase* converter = sJSONFromNumberConverters[i];
		if ( converter->AcceptsProperty(Prop) )
		{
			converter->ExportValue( JObjValue, PropertyValue );
			return;
		}
	}

	// Handle non-number properties.
	if ( Prop->IsA( UBoolProperty::StaticClass()) )
	{
		ExportPropertyBool( JObjValue, Cast<UBoolProperty>(Prop), PropertyValue, DefaultValue, Parent, PortFlags, ExportRootScope );		
	}
	else if ( Prop->IsA( UByteProperty::StaticClass()) )
	{
		ExportPropertyByte( JObjValue, Cast<UByteProperty>(Prop), PropertyValue, DefaultValue, Parent, PortFlags, ExportRootScope );		
	}
	else if ( Prop->IsA( UObjectPropertyBase::StaticClass()) )
	{
		// Decide whether this is a referenced asset pointer, or an inline-exported object.
		if ( (Prop->PropertyFlags & CPF_AssetRegistrySearchable) != 0 )
		{
			ExportPropertyAssetReference( JObjValue, Cast<UObjectPropertyBase>(Prop), PropertyValue, DefaultValue, Parent, PortFlags, ExportRootScope );		
		}
		else
		{
			// Inline-import based on class name.
			ExportPropertySubObject( JObjValue, Cast<UObjectPropertyBase>(Prop), PropertyValue, DefaultValue, Parent, PortFlags, ExportRootScope );
		}
	}
	else if ( Prop->IsA( UStructProperty::StaticClass())  )
	{
		// Contained struct.
		ExportPropertyStruct( JObjValue, Cast<UStructProperty>(Prop), PropertyValue, DefaultValue, Parent, PortFlags, ExportRootScope );
	}
	else if ( Prop->IsA( UArrayProperty::StaticClass()) )
	{
		ExportPropertyArray( JObjValue, Cast<UArrayProperty>(Prop), PropertyValue, DefaultValue, Parent, PortFlags, ExportRootScope );		
	}
	else
	{
		ExportPropertyString( JObjValue, Prop, PropertyValue, DefaultValue, Parent, PortFlags, ExportRootScope );		
	}
}

void FJsonExporter::ExportPropertyBool( TSharedPtr<FJsonValue>& JObjValue, UBoolProperty* Prop, const uint8* PropertyValue, const uint8* DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope )
{
	FString ValueStr;
	Prop->ExportTextItem( ValueStr, PropertyValue, DefaultValue, Parent, PortFlags, ExportRootScope );
	if ( ValueStr != TEXT("True") && ValueStr != TEXT("False") )
	{
		UE_LOG(LogJsonUtils, Error, TEXT("Value: %s, Expected boolean when exporting property %s from JSON"), *ValueStr, *Prop->GetName());
		return;
	}
	JObjValue = MakeShareable(new FJsonValueBoolean( ValueStr == TEXT("True") ));
}

void FJsonExporter::ExportPropertyByte( TSharedPtr<FJsonValue>& JObjValue, UByteProperty* Prop, const uint8* PropertyValue, const uint8* DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope )
{
	FString ValueStr;
	Prop->ExportTextItem( ValueStr, PropertyValue, DefaultValue, Parent, PortFlags, ExportRootScope );
	JObjValue = MakeShareable(new FJsonValueString( ValueStr ));
}

void FJsonExporter::ExportPropertyAssetReference( TSharedPtr<FJsonValue>& JObjValue, UObjectPropertyBase* Prop, const uint8* PropertyValue, const uint8* DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope )
{
	FString ValueStr;
	// Export Asset Name
	Prop->ExportTextItem( ValueStr, PropertyValue, DefaultValue, Parent, PortFlags, ExportRootScope );
	JObjValue = MakeShareable(new FJsonValueString( ValueStr ));
}

void FJsonExporter::ExportPropertySubObject( TSharedPtr<FJsonValue>& JObjValue, UObjectPropertyBase* Prop, const uint8* PropertyValue, const uint8* DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope )
{
	UObject* Object = Prop->GetObjectPropertyValue(PropertyValue);
	if (JObjValue.IsValid())
		FJsonExporter::Export( JObjValue, Object->GetClass(), (uint8*)Object, Object->GetOuter(), PPF_ExportsNotFullyQualified|PPF_Copy, true );	
}

void FJsonExporter::ExportPropertyStruct( TSharedPtr<FJsonValue>& JObjValue, UStructProperty* Prop, const uint8* PropertyValue, const uint8* DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope )
{
	// Export Struct as JsonObject
	Export( JObjValue, Prop->Struct, PropertyValue, Parent, PortFlags, false );	
}

void FJsonExporter::ExportPropertyArray( TSharedPtr<FJsonValue>& JObjValue, UArrayProperty* Prop, const uint8* PropertyValue, const uint8* DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope )
{
	FScriptArrayHelper ArrayHelper(Prop, PropertyValue);
	FScriptArrayHelper DefaultArrayHelper(Prop, DefaultValue);

	TArray< TSharedPtr<FJsonValue> > ArrayJObj;

	uint8* StructDefaults = NULL;
	UStructProperty* StructProperty = Cast<UStructProperty>(Prop->Inner);
	if ( StructProperty != nullptr )
	{
		checkSlow(StructProperty->Struct);
		StructDefaults = (uint8*)FMemory::Malloc(StructProperty->Struct->GetStructureSize());
		StructProperty->InitializeValue(StructDefaults);
	}

	int32 Count = 0;
	for( int32 i=0; i<ArrayHelper.Num(); i++ )
	{
		uint8* PropData = ArrayHelper.GetRawPtr(i);
		uint8* PropDefault = (DefaultValue && DefaultArrayHelper.Num() > i) ? DefaultArrayHelper.GetRawPtr(i) : StructDefaults;

		// Do not re-export duplicate data from superclass when exporting to .int32 file
		if ( Prop->Inner->Identical(PropData, PropDefault) )
		{
			continue;
		}
		TSharedPtr<FJsonValue> InnerJObj;
		ExportProperty( InnerJObj, Prop->Inner, PropData, PropDefault, Parent, PortFlags, i, ExportRootScope );		
		if ( InnerJObj.IsValid() )
		{
			ArrayJObj.Add( InnerJObj );
		}
	}

	if (StructDefaults)
	{
		StructProperty->DestroyValue(StructDefaults);
		FMemory::Free(StructDefaults);
	}

	JObjValue = MakeShareable(new FJsonValueArray(ArrayJObj));
}

void FJsonExporter::ExportPropertyString( TSharedPtr<FJsonValue>& JObjValue, UProperty* Prop, const uint8* PropertyValue, const uint8* DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope )
{
	FString ValueStr;
	Prop->ExportTextItem( ValueStr, PropertyValue, DefaultValue, Parent, PortFlags, ExportRootScope );
	JObjValue = MakeShareable(new FJsonValueString( ValueStr ));
}

// Un-define local define(s), since we don't want them polluting the bunched cpp after this.
#undef LIST_SIMPLE_PROPERTY_TYPES

#undef LOC_NAMESPACE