// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SessionEntryData.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSION_API USessionEntryData : public UObject
{
	GENERATED_BODY()
	
public:
	USessionEntryData();

public:
	UPROPERTY(BlueprintReadOnly)
	FString SessionName;

	UPROPERTY(BlueprintReadOnly)
	FString MapName;

	UPROPERTY(BlueprintReadOnly)
	FString UserName;

	UPROPERTY(BlueprintReadOnly)
	int32 MaxPlayerNum;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentPlayerNum;

	UPROPERTY(BlueprintReadOnly)
	int32 Ping;

	UPROPERTY(BlueprintReadOnly)
	int32 SessionSearchResultIndex;
};
