// Copyright (C) Microsoft. All rights reserved.

#include "JsonUtilsPrivatePCH.h"

#include "JsonUtilCommon.h"
#include "JsonImporter.h"
#include "Json.h"

#define LOC_NAMESPACE TEXT("JsonUtils")

// Utilities to copy number properties from a JSON object to the actual property.

class FJsonToNumberPropertyBase
{
public:

	virtual bool AcceptsProperty( UProperty* Prop ) const = 0;

	bool ReadFromJSON( const TSharedPtr<FJsonValue>& JObjValue, UProperty* Prop, uint8* Data, int32 PortFlags, UObject* Parent, FOutputDevice* ErrorText = (FOutputDevice*)GWarn ) const;

protected:

	virtual void CopyValue( const TSharedPtr<FJsonValue>& JObjValue, UProperty* Prop, uint8* DestData ) const = 0;
};

bool FJsonToNumberPropertyBase::ReadFromJSON( const TSharedPtr<FJsonValue>& JObjValue, UProperty* Prop, uint8* Data, int32 PortFlags, UObject* Parent, FOutputDevice* ErrorText ) const
{
	if ( !(JObjValue.IsValid() && JObjValue->Type == EJson::Number) )
	{
		UE_LOG(LogJsonUtils, Error, TEXT("Expected number when importing property %s from JSON"), *Prop->GetName());
		return false;
	}
	CopyValue( JObjValue, Prop, Data );

	return true;
}

// Template implementation of the above.
template <typename T>
class FJsonToNumberProperty : public FJsonToNumberPropertyBase
{
protected:

	typedef typename typename FJsonNumberPropertyTraits<T>::PropertyType PropertyType;

	bool AcceptsProperty( UProperty* Prop ) const
	{
		return Prop->IsA( PropertyType::StaticClass() );
	}

	virtual void CopyValue( const TSharedPtr<FJsonValue>& JObjValue, UProperty* Prop, uint8* DestData ) const
	{
		T src = static_cast<T>(JObjValue->AsNumber());
		Prop->CopySingleValue( DestData, &src );
	}
};

// FJsonImporter:
// Utility to import a UObject from JSON.

bool FJsonImporter::ImportDataFromJson( uint8*& ObjectData, UStruct* ObjectClass, UObject* Parent, const TSharedPtr<class FJsonObject>& JSONObject, int32 PortFlags )
{
	if ( !JSONObject.IsValid() )
	{
		UE_LOG(LogJsonUtils, Error, TEXT("Got invalid JSON object after parsing JSON string"));
		return false;
	}

	if (ObjectData == nullptr)
	{
		// Create the object from Json
		const TSharedPtr<FJsonValue>* ClassValue = JSONObject->Values.Find(TEXT("Class"));
		if ( !(ClassValue != nullptr && ClassValue->IsValid() && (*ClassValue)->Type == EJson::String) )
		{
			UE_LOG(LogJsonUtils, Error, TEXT("NULL object data when importing from JSON"));
			return false;
		}

		FString ClassNameString = (*ClassValue)->AsString();
		UObject* ClassObj = StaticFindObject( UClass::StaticClass(), ANY_PACKAGE, *ClassNameString );
		if( !ClassObj )
		{
			UE_LOG(LogJsonUtils, Error, TEXT("Invalid class name %s for object when importing %s from JSON"), *ClassNameString );
			return false;
		}

		UClass* ThisClass = Cast<UClass>(ClassObj);
		if( !ThisClass )
		{
			UE_LOG(LogJsonUtils, Error, TEXT("Invalid class name %s forobject when importing property %s from JSON - object should be a class"), *ClassNameString );
			return false;
		}

		UObject* CreatedObject = NewObject<UObject>( Parent, FName(NAME_None));

		if ( !CreatedObject )
		{
			UE_LOG(LogJsonUtils, Error, TEXT("NULL object data when importing from JSON"));
			return false;
		}

		ObjectData = (uint8*)CreatedObject;		
		ObjectClass = ThisClass;
	}

	if (ObjectClass == nullptr)
	{
		UE_LOG(LogJsonUtils, Error, TEXT("Invalid class on UObject when importing from JSON"));
		return false;
	}

	return Import( ObjectData, ObjectClass, JSONObject, Parent, PortFlags, (FOutputDevice*)GWarn );
}

bool FJsonImporter::ImportDataFromJsonString( uint8*& ObjectData, UStruct* ObjectClass, UObject* Parent, const FString& JSONString, int32 PortFlags )
{
	TSharedPtr<FJsonObject> JSONObject;
	FString OutError;
	TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create( JSONString );
	bool Success = FJsonSerializer::Deserialize( Reader, JSONObject );
	if ( !Success )
	{
		UE_LOG(LogJsonUtils, Error, TEXT("Error parsing JSON string: %s"), *Reader->GetErrorMessage());
		return false;
	}

	Success = ImportDataFromJson( ObjectData, ObjectClass, Parent, JSONObject, PortFlags );
	return Success;
}

bool FJsonImporter::Import( uint8* ObjectData, UStruct* ObjectClass, const TSharedPtr<FJsonObject>& JObj, UObject* Parent, int32 PortFlags, FOutputDevice* ErrorText )
{
	// Read properties one by one.
	bool success = true;
	// Set property for the new object
	for( UProperty* Property = ObjectClass->PropertyLink; Property; Property = Property->PropertyLinkNext )
	{
		if (Property->ShouldPort(PortFlags))
		{
			if ( !Property->ValidateImportFlags(PortFlags,ErrorText) )
			{
				UE_LOG(LogJsonUtils, Error, TEXT("Invalid import flags for property %s when importing from JSON"), *Property->GetName());
				success = false;
				break;
			}

			const TSharedPtr<FJsonValue>* JObjValue = JObj->Values.Find( Property->GetName() );
			if (JObjValue == nullptr)
			{
				// If the property is not present in JSON file,
				// that's ok, we just depend on the default value and don't worry about it here.
				continue;
			}

			/*if ( (PortFlags & PPF_LocalizedOnly) != 0 && !Property->IsLocalized() )
			{
				UE_LOG(LogJsonUtils, Error, TEXT("Cannot import property %s from JSON - flags are invalid"), *Property->GetName());
				success = false;
				break;
			}*/

			if ( !ImportProperty(
				*JObjValue, 
				Property,
				Property->ContainerPtrToValuePtr<uint8>(ObjectData),
				Parent,
				PortFlags,
				-1,
				ErrorText ) )
			{
				success = false;
				break;
			}
		}
	}
	return success;
}

bool FJsonImporter::ImportProperty( const TSharedPtr<FJsonValue>& JObjValue, UProperty* Prop, uint8* PropertyValue, UObject* Parent, int32 PortFlags, int32 ArrayIndex, FOutputDevice* ErrorText )
{
	bool success = true;

// Static list of number converters.

#define STATIC_JSON_CONVERTER(CppType, UEType ) \
	static const FJsonToNumberProperty<CppType> sJSONToNumber_##UEType;

LIST_SIMPLE_PROPERTY_TYPES( STATIC_JSON_CONVERTER )

#undef STATIC_JSON_CONVERTER

#define STATIC_JSON_CONVERTER_ELEMENT(CppType, UEType ) &sJSONToNumber_##UEType,

	// A corresponding static list of converter base class pointers
	static const FJsonToNumberPropertyBase* sJSONToNumberConverters[] =
	{
		LIST_SIMPLE_PROPERTY_TYPES(STATIC_JSON_CONVERTER_ELEMENT)
	};

#undef STATIC_JSON_CONVERTER_ELEMENT

	// Handle number properties.
	static const uint32 sNumConverters = sizeof(sJSONToNumberConverters)/sizeof(sJSONToNumberConverters[0]);
	bool handled = false;
	for ( uint32 i=0 ; i < sNumConverters ; ++i )
	{
		const FJsonToNumberPropertyBase* converter = sJSONToNumberConverters[i];
		if ( converter->AcceptsProperty(Prop) )
		{
			success = converter->ReadFromJSON(JObjValue, Prop, PropertyValue, PortFlags, Parent, ErrorText);
			handled = true;
			break;
		}
	}

	if ( handled )
	{
		return success;
	}

	// Handle non-number properties.

	if ( Prop->IsA( UByteProperty::StaticClass()) )
	{
		success = ImportPropertyByte( JObjValue, Cast<UByteProperty>(Prop), PropertyValue, PortFlags, Parent, ErrorText );	
	}
	else if ( Prop->IsA( UBoolProperty::StaticClass()) )
	{
		success = ImportPropertyBool( JObjValue, Cast<UBoolProperty>(Prop), PropertyValue, PortFlags, Parent, ErrorText );	
	}
	else if ( Prop->IsA( UObjectPropertyBase::StaticClass())  )
	{
		// Decide whether this is a referenced asset pointer, or an inline-exported object.
		if ( (Prop->PropertyFlags & CPF_AssetRegistrySearchable) != 0 )
		{
			// Referenced asset.
			success = ImportPropertyAssetReference( JObjValue, Cast<UObjectPropertyBase>(Prop), PropertyValue, PortFlags, Parent, ErrorText );
		}
		else
		{
			// Inline-import based on class name.
			success = ImportPropertySubObject( JObjValue, Cast<UObjectPropertyBase>(Prop), PropertyValue, PortFlags, Parent, ErrorText );
		}
	}
	else if ( Prop->IsA( UStructProperty::StaticClass())  )
	{
		// Contained struct.
		success = ImportPropertyStruct( JObjValue, Cast<UStructProperty>(Prop), PropertyValue, PortFlags, Parent, ErrorText );
	}
	else if ( Prop->IsA( UArrayProperty::StaticClass()) )
	{
		success = ImportPropertyArray( JObjValue, Cast<UArrayProperty>(Prop), PropertyValue, PortFlags, Parent, ErrorText );	
	}
	else
	{
		success = ImportPropertyString( JObjValue, Prop, PropertyValue, PortFlags, Parent, ErrorText );		
	}

	return success;
}

bool FJsonImporter::ImportPropertyByte( const TSharedPtr<class FJsonValue>& JObjValue, UByteProperty* Prop, uint8* Data, int32 PortFlags, UObject* Parent, FOutputDevice* ErrorText )
{
	if ( !(JObjValue.IsValid() && JObjValue->Type == EJson::String) )
	{
		UE_LOG(LogJsonUtils, Error, TEXT("Expected enum string when importing property %s from JSON"), *Prop->GetName());
		return false;
	}
	if (Prop->Enum == nullptr)
	{
		UE_LOG(LogJsonUtils, Error, TEXT("Expected enum declaration for property %s when importing it from JSON"), *Prop->GetName());
		return false;
	}

	FString EnumString = JObjValue->AsString();

	// Copy-pasting code in UByteProperty::ImportText, to parse enums.
	const int32 EnumIndex = Prop->Enum->FindEnumIndex( *EnumString );
	if( EnumIndex == INDEX_NONE )
	{
		UE_LOG(LogJsonUtils, Error, TEXT("Unknown enum value %s when importing property %s from JSON"), *EnumString, *Prop->GetName());
	}
	*(uint8*)Data = EnumIndex;

	return true;
}

bool FJsonImporter::ImportPropertyBool( const TSharedPtr<FJsonValue>& JObjValue, UBoolProperty* Prop, uint8* Data, int32 PortFlags, UObject* Parent, FOutputDevice* ErrorText )
{
	if ( !(JObjValue.IsValid() && JObjValue->Type == EJson::Boolean) )
	{
		UE_LOG(LogJsonUtils, Error, TEXT("Expected boolean when importing property %s from JSON"), *Prop->GetName());
		return false;
	}
	bool src = JObjValue->AsBool();
	Prop->CopySingleValue( Data, &src );

	return true;
}

bool FJsonImporter::ImportPropertyAssetReference( const TSharedPtr<FJsonValue>& JObjValue, UObjectPropertyBase* Prop, uint8* Data, int32 PortFlags, UObject* Parent, FOutputDevice* ErrorText )
{
	if ( !(JObjValue.IsValid() && JObjValue->Type == EJson::String) )
	{
		UE_LOG(LogJsonUtils, Error, TEXT("Expected asset reference string when importing property %s from JSON"), *Prop->GetName());
		return false;
	}
	FString AssetNameString = JObjValue->AsString();
	if (AssetNameString.IsEmpty())
	{
		Prop->SetObjectPropertyValue(Data, nullptr);
	}
	else
	{
		UObject* AssetObj = StaticFindObject( Prop->PropertyClass, ANY_PACKAGE, *AssetNameString );
		if( !AssetObj )
		{
			UE_LOG(LogJsonUtils, Error, TEXT("Could not find asset %s when importing property %s from JSON"), *AssetNameString, *Prop->GetName());
			return false;
		}

		if ( !AssetObj->IsA(Prop->PropertyClass) )
		{
			// Wrong class of asset.
			UE_LOG(LogJsonUtils, Error, TEXT("Wrong class name for asset %s when importing property %s from JSON"), *AssetNameString, *Prop->GetName());
			return false;
		}
		Prop->SetObjectPropertyValue(Data, AssetObj);
	}

	return true;
}

bool FJsonImporter::ImportPropertySubObject( const TSharedPtr<FJsonValue>& JObjValue, UObjectPropertyBase* Prop, uint8* Data, int32 PortFlags, UObject* Parent, FOutputDevice* ErrorText )
{
	if ( !( JObjValue.IsValid() && (JObjValue->Type == EJson::Object || JObjValue->Type == EJson::String) ) )
	{
		UE_LOG(LogJsonUtils, Error, TEXT("Expected string (null) or Class-Object pair when importing sub-object property %s from JSON"), *Prop->GetName());
		return false;
	}

	if (JObjValue->Type == EJson::String)
	{
		// Better be the string "null".
		FString ValueString = JObjValue->AsString();
		if (ValueString != "null")
		{
			UE_LOG(LogJsonUtils, Error, TEXT("Expected the \"null\" string but found \"%s\" when importing sub-object property %s from JSON"), *ValueString, *Prop->GetName());
			return false;
		}

		Prop->SetObjectPropertyValue(Data, nullptr);

		return true;
	}

	TSharedPtr<FJsonObject> SpecJObj = JObjValue->AsObject();
	if (!JObjValue.IsValid())
	{
		UE_LOG(LogJsonUtils, Error, TEXT("Expected valid Class-Object pair when importing sub-object property %s from JSON"), *Prop->GetName());
		return false;
	}

	const TSharedPtr<FJsonValue>* ClassValue = SpecJObj->Values.Find(TEXT("Class"));
	if ( !(ClassValue != nullptr && ClassValue->IsValid() && (*ClassValue)->Type == EJson::String) )
	{
		UE_LOG(LogJsonUtils, Error, TEXT("Could not find class name for sub-object when importing property %s from JSON"), *Prop->GetName());
		return false;
	}

	FString ClassNameString = (*ClassValue)->AsString();
	UObject* ClassObj = StaticFindObject( UClass::StaticClass(), ANY_PACKAGE, *ClassNameString );
	if( !ClassObj )
	{
		UE_LOG(LogJsonUtils, Error, TEXT("Invalid class name %s for sub-object when importing property %s from JSON"), *ClassNameString, *Prop->GetName());
		return false;
	}

	UClass* ObjectClass = Cast<UClass>(ClassObj);
	if( !ObjectClass )
	{
		UE_LOG(LogJsonUtils, Error, TEXT("Invalid class name %s for sub-object when importing property %s from JSON - object should be a class"), *ClassNameString, *Prop->GetName());
		return false;
	}

	if ( !ObjectClass->IsChildOf(Prop->PropertyClass) )
	{
		UE_LOG(LogJsonUtils, Error, TEXT("Invalid class name %s for sub-object when importing property %s from JSON - class should be %s or its subclass"), *ClassNameString, *Prop->GetName(), *Prop->PropertyClass->GetName());
		return false;
	}

	TSharedPtr<FJsonObject> ObjectJObj  = SpecJObj;	
	const TSharedPtr<FJsonValue>* ObjectValue = SpecJObj->Values.Find(TEXT("Object"));
	// The SubObject is inside Object{ ... }
	if ( ObjectValue != nullptr && ObjectValue->IsValid() )
	{
		if ( (*ObjectValue)->Type != EJson::Object )
		{
			UE_LOG(LogJsonUtils, Error, TEXT("Could not find object value for sub-object when importing property %s from JSON"), *Prop->GetName());
			return false;
		}
		ObjectJObj = (*ObjectValue)->AsObject();
	}		

	// Create the object 
	UObject* CreatedObject = NewObject<UObject>(Parent, FName(NAME_None), RF_Transactional);

	if ( !Import((uint8*)CreatedObject, ObjectClass, ObjectJObj, Parent, PortFlags, ErrorText) )
	{
		return false;
	}

	Prop->SetObjectPropertyValue(Data, CreatedObject);
	return true;
}

bool FJsonImporter::ImportPropertyStruct( const TSharedPtr<FJsonValue>& JObjValue, UStructProperty* Prop, uint8* Data, int32 PortFlags, UObject* Parent, FOutputDevice* ErrorText)
{
	checkSlow(Prop->Struct);

	if ( !(JObjValue.IsValid() && JObjValue->Type == EJson::Object) )
	{
		UE_LOG(LogJsonUtils, Error, TEXT("Expected object when importing struct property %s from JSON"), *Prop->GetName());
		return false;
	}

	TSharedPtr<FJsonObject> StructJObj = JObjValue->AsObject();

	// Recurse.
	return Import(Data, Prop->Struct, StructJObj, Parent, PortFlags, ErrorText);
}

bool FJsonImporter::ImportPropertyArray( const TSharedPtr<FJsonValue>& JObjValue, UArrayProperty* Prop, uint8* Data, int32 PortFlags, UObject* Parent, FOutputDevice* ErrorText )
{
	checkSlow(Prop->Inner);
	
	if ( !(JObjValue.IsValid() && JObjValue->Type == EJson::Array) )
	{
		UE_LOG(LogJsonUtils, Error, TEXT("Expected array value when importing property %s from JSON"), *Prop->GetName());
		return false;
	}
	const TArray< TSharedPtr<FJsonValue> > ArrayJObj = JObjValue->AsArray();

	FScriptArrayHelper ArrayHelper(Prop, Data);
	/*// only clear the array if we're not importing localized text
	if ( (PortFlags&PPF_LocalizedOnly) == 0 )
	{
		ArrayHelper.EmptyValues();
	}*/

	for (int32 i = 0; i < ArrayJObj.Num(); ++i)
	{
		const TSharedPtr<FJsonValue>& InnerValue = ArrayJObj[i];
		if ( !InnerValue.IsValid())
		{
			UE_LOG(LogJsonUtils, Error, TEXT("Expected valid array element at index %d when importing property %s from JSON"), i, *Prop->GetName());
			return false;
		}

		ArrayHelper.ExpandForIndex(i);		
		ImportProperty( InnerValue, Prop->Inner, ArrayHelper.GetRawPtr(i), Parent, PortFlags, i, ErrorText );
	}

	return true;
}

bool FJsonImporter::ImportPropertyString( const TSharedPtr<FJsonValue>& JObjValue, UProperty* Prop, uint8* Data, int32 PortFlags, UObject* Parent, FOutputDevice* ErrorText )
{
	if ( !(JObjValue.IsValid() && JObjValue->Type == EJson::String) )
	{
		UE_LOG(LogJsonUtils, Error, TEXT("Expected string when importing property %s from JSON (or the property type is not supported)"), *Prop->GetName());
		return false;
	}
	FString Buffer = JObjValue->AsString();
	Prop->ImportText( *Buffer, Data, PortFlags, Parent, ErrorText );

	return true;
}

// Un-define local define(s), since we don't want them polluting the bunched cpp after this.
#undef LIST_SIMPLE_PROPERTY_TYPES

#undef LOC_NAMESPACE
