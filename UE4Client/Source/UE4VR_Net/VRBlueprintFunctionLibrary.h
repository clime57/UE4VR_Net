// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "VRBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UE4VR_NET_API UVRBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Game")
		static void ComputeCPUGPUPerfIndex(float& CPUIndex, float& GPUIndex);

	UFUNCTION(BlueprintCallable, Category = "File")
		static bool FileSaveString(FString SaveTextB, FString FileNameB);

	UFUNCTION(BlueprintPure, Category = "File")
		static bool FileLoadString(FString FileNameA, FString& SaveTextA);

	UFUNCTION(BlueprintPure, Category = "Game|Localization")
		static int32 GetLocalizationType();

	UFUNCTION(BlueprintCallable, Category = "Game|Localization")
		static void SetLocalizationType(const int32 LocalType);

	UFUNCTION(BlueprintCallable, Category = "Game")
		static FString GetToken();

	UFUNCTION(BlueprintCallable, Category = "Game")
		static FString GetGameCmdLine();
};
