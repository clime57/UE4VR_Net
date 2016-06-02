// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4VR_Net.h"
#include "VRBlueprintFunctionLibrary.h"
#include "SynthBenchmark.h"
#include "VRGameUserSettings.h"

void UVRBlueprintFunctionLibrary::ComputeCPUGPUPerfIndex(float& CPUIndex, float& GPUIndex)
{
	FSynthBenchmarkResults SynthBenchmark;
	ISynthBenchmark::Get().Run(SynthBenchmark, true, 10U);

	CPUIndex = SynthBenchmark.ComputeCPUPerfIndex();
	GPUIndex = SynthBenchmark.ComputeGPUPerfIndex();
}

bool UVRBlueprintFunctionLibrary::FileSaveString(FString SaveTextB, FString FileNameB)
{
	return FFileHelper::SaveStringToFile(SaveTextB, *(FPaths::GameDir() + FileNameB));
}

bool UVRBlueprintFunctionLibrary::FileLoadString(FString FileNameA, FString& SaveTextA)
{
	return FFileHelper::LoadFileToString(SaveTextA, *(FPaths::GameDir() + FileNameA));
}


int32 UVRBlueprintFunctionLibrary::GetLocalizationType()
{
	int32 localType = 0;
	UVRGameUserSettings* UserSetting = Cast<UVRGameUserSettings>(GEngine->GetGameUserSettings());
	if (UserSetting != nullptr)
	{
		//localType = UserSetting->LocalizationType;
	}
	return localType;
}


void UVRBlueprintFunctionLibrary::SetLocalizationType(const int32 LocalType)
{
	if (LocalType < 0)
	{
		return;
	}
	UVRGameUserSettings* UserSetting = Cast<UVRGameUserSettings>(GEngine->GetGameUserSettings());
	if (UserSetting != nullptr)
	{
		//UserSetting->LocalizationType = LocalType;
		UserSetting->SaveSettings();
	}
}


FString UVRBlueprintFunctionLibrary::GetToken()
{
	return GetGameCmdLine();
}

FString UVRBlueprintFunctionLibrary::GetGameCmdLine()
{
	const TCHAR * cmdline = FCommandLine::Get();
	return cmdline;
}