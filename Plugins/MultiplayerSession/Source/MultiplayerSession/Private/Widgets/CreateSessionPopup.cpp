// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CreateSessionPopup.h"

#include "Animation/WidgetAnimation.h"
#include "Subsystems/MultiplayerSessionSubsystem.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableText.h"

UCreateSessionPopup::UCreateSessionPopup(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PopupSize = FVector2D(300.0f, 380.0f);
}

void UCreateSessionPopup::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(Button_Create))
	{
		Button_Create->OnReleased.AddDynamic(this, &UCreateSessionPopup::UCreateSessionPopup::OnCreateButtonReleased);
	}

	if (IsValid(Button_Back))
	{
		Button_Back->OnReleased.AddDynamic(this, &UCreateSessionPopup::OnBackButtonReleased);
	}

	if (IsValid(FloatingAnimation))
	{
		PlayAnimation(FloatingAnimation, 0.0f, 1);
	}
}

void UCreateSessionPopup::OnCreateButtonReleased()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance))
	{
		UMultiplayerSessionSubsystem* Subsystem = GameInstance->GetSubsystem<UMultiplayerSessionSubsystem>();
		if (IsValid(Subsystem))
		{
			FString SessionName = EditableText_SessionName->GetText().ToString();
			FString MapName = ComboBox_Map->GetSelectedOption();
			uint32 NumPublicConnections = FCString::Atoi(*ComboBox_Slots->GetSelectedOption());
			
			Subsystem->CreateSession(SessionName, MapName, NumPublicConnections);
		}
	}
}

void UCreateSessionPopup::OnBackButtonReleased()
{
	OnRemoveFromParent.Broadcast();	
	RemoveFromParent();
}
