// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SessionMenu.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Widgets/CreateSessionPopup.h"
#include "Widgets/JoinSessionPopup.h"

void USessionMenu::MenuSetup()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	SetIsFocusable(true);

	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController != nullptr)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}

	if (IsValid(Button_Create))
	{
		Button_Create->OnReleased.AddDynamic(this, &USessionMenu::OnCreateButtonReleased);
	}

	if (IsValid(Button_Join))
	{
		Button_Join->OnReleased.AddDynamic(this, &USessionMenu::OnJoinButtonReleased);
	}

	if (IsValid(Button_Quit))
	{
		Button_Quit->OnReleased.AddDynamic(this, &USessionMenu::OnQuitButtonReleased);
	}
}

void USessionMenu::OnCreateButtonReleased()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		return;
	}
	
	UCreateSessionPopup* CreateSessionPopup = CreateWidget<UCreateSessionPopup>(PlayerController, CreateSessionPopupClass);

	if (IsValid(CreateSessionPopup))
	{
		CreateSessionPopup->AddToViewport();
		
		FVector2D ViewportSize;
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		ViewportSize *= 0.5f;

		FVector2D PopupSize = CreateSessionPopup->GetPopupSize();
		float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(PlayerController);
		PopupSize *= 0.5f;
		PopupSize *= ViewportScale;

		const FVector2D PopupPosition = ViewportSize - PopupSize;
		CreateSessionPopup->SetPositionInViewport(PopupPosition, true);
		CreateSessionPopup->OnRemoveFromParent.AddLambda([this]
		{
			VerticalBox_Button->SetVisibility(ESlateVisibility::Visible);
		});

		VerticalBox_Button->SetVisibility(ESlateVisibility::Collapsed); 
	}
}

void USessionMenu::OnJoinButtonReleased()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		return;
	}

	UJoinSessionPopup* JoinSessionPopup = CreateWidget<UJoinSessionPopup>(PlayerController, JoinSessionPopupClass);
	
	if (IsValid(JoinSessionPopup))
	{
		JoinSessionPopup->AddToViewport();
		
		FVector2D ViewportSize;
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		ViewportSize *= 0.5f;

		FVector2D PopupSize = JoinSessionPopup->GetPopupSize();
		float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(PlayerController);
		PopupSize *= 0.5f;
		PopupSize *= ViewportScale;

		const FVector2D PopupPosition = ViewportSize - PopupSize;
		JoinSessionPopup->SetPositionInViewport(PopupPosition, true);
		JoinSessionPopup->OnRemoveFromParent.AddLambda([this]
		{
			VerticalBox_Button->SetVisibility(ESlateVisibility::Visible);
		});
		
		VerticalBox_Button->SetVisibility(ESlateVisibility::Collapsed); 
	}
}

void USessionMenu::OnQuitButtonReleased()
{
	// @Todo_JYH: 세션 메뉴 종료 처리(기획에 따라 다르게 처리).
}
