// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TargetableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTargetableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *	Author: 지용현
 *	Date: 2026.01.08
 *
 *	하드 타겟팅 
 */
class HARDTARGETINGSYSTEM_API ITargetableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnHovered() = 0;
	virtual void OnUnhovered() = 0;
	virtual void OnSelected() = 0;
	virtual void OnDeselected() = 0;

	virtual void GetTargetBounds(float& OutHalfWidth, float& OutHalfHeight) const = 0;
};
