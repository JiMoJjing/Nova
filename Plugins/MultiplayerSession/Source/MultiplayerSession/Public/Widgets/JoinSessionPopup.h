// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JoinSessionPopup.generated.h"

class UListView;
DECLARE_MULTICAST_DELEGATE(FOnRemoveFromParent);

class UMultiplayerSessionSubsystem;
class UButton;
class UEditableText;
/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSION_API UJoinSessionPopup : public UUserWidget
{
	GENERATED_BODY()

public:
	UJoinSessionPopup(const FObjectInitializer& ObjectInitializer);
	void NativeConstruct() override;

	FORCEINLINE FVector2D GetPopupSize() const { return PopupSize; }

private:
	void OnFindSession(const TArray<FOnlineSessionSearchResult>& SearchResults, bool bWasSuccessful);

	UFUNCTION()
	void OnBackButtonReleased();

	UFUNCTION()
	void OnSearchButtonReleased();

	UFUNCTION()
	void OnRefreshButtonReleased();

	UFUNCTION()
	void OnFloatingAnimationFinished();

public:
	FOnRemoveFromParent OnRemoveFromParent;

private:
	UPROPERTY()
	TObjectPtr<UMultiplayerSessionSubsystem> MultiplayerSessionSubsystem;
	
	UPROPERTY(BlueprintReadOnly, Category = "JoinSessionPopup", meta = (BindWidget = "EditableText_SessionName", AllowPrivateAccess = "true"))
	TObjectPtr<UEditableText> EditableText_SessionName;

	UPROPERTY(BlueprintReadOnly, Category = "JoinSessionPopup", meta = (BindWidget = "Button_Back", AllowPrivateAccess = "true"))
	TObjectPtr<UButton> Button_Back;
	
	UPROPERTY(BlueprintReadOnly, Category = "JoinSessionPopup", meta = (BindWidget = "Button_Search", AllowPrivateAccess = "true"))
	TObjectPtr<UButton> Button_Search;

	UPROPERTY(BlueprintReadOnly, Category = "JoinSessionPopup", meta = (BindWidget = "Button_Refresh", AllowPrivateAccess = "true"))
	TObjectPtr<UButton> Button_Refresh;

	UPROPERTY(BlueprintReadOnly, Category = "JoinSessionPopup", meta = (BindWidget = "ListView_Sessions", AllowPrivateAccess = "true"))
	TObjectPtr<UListView> ListView_Sessions;

	UPROPERTY(BlueprintReadOnly, Category = "JoinSessionPopup", Transient, meta = (BindWidgetAnim = "FloatingAnimation", AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetAnimation> FloatingAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CreateSessionPopup", meta = (AllowPrivateAccess = "true"))
	FVector2D PopupSize;

};
