// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionMenu.generated.h"

class UJoinSessionPopup;
class UVerticalBox;
class UButton;
class UMultiplayerSessionSubsystem;
class UCreateSessionPopup;
/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSION_API USessionMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void MenuSetup();

private:	
	UFUNCTION()
	void OnCreateButtonReleased();

	UFUNCTION()
	void OnJoinButtonReleased();

	UFUNCTION()
	void OnQuitButtonReleased();
	
	
private:
	UPROPERTY()
	TObjectPtr<UMultiplayerSessionSubsystem> MultiplayerSessionSubsystem;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SessionMenu", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UCreateSessionPopup> CreateSessionPopupClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SessionMenu", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UJoinSessionPopup> JoinSessionPopupClass;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true", BindWidget = "VerticalBox_Button"))
	TObjectPtr<UVerticalBox> VerticalBox_Button;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true", BindWidget = "Button_Create"))
	TObjectPtr<UButton> Button_Create;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true", BindWidget = "Button_Join"))
	TObjectPtr<UButton> Button_Join;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true", BindWidget = "Button_Quit"))
	TObjectPtr<UButton> Button_Quit;
};
