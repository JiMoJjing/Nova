// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/NovaInputComponent.h"



UNovaInputComponent::UNovaInputComponent(const FObjectInitializer& ObjectInitializer)
{

}

void UNovaInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (uint32 Handle : BindHandles)
	{
		RemoveBindingByHandle(Handle);
	}
	BindHandles.Reset();
}

