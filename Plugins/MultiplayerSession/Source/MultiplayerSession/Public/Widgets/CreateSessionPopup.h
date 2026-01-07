// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreateSessionPopup.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnRemoveFromParent);

class UButton;
class UComboBoxString;
class UEditableText;
/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSION_API UCreateSessionPopup : public UUserWidget
{
	GENERATED_BODY()

public:
	UCreateSessionPopup(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeConstruct() override;

	FORCEINLINE FVector2D GetPopupSize() const { return PopupSize; }

private:
	UFUNCTION()
	void OnCreateButtonReleased();

	UFUNCTION()
	void OnBackButtonReleased();

public:
	FOnRemoveFromParent OnRemoveFromParent;

private:
	UPROPERTY(meta = (BindWidget = "EditableText_SessionName"))
	TObjectPtr<UEditableText> EditableText_SessionName;

	UPROPERTY(meta = (BindWidget = "ComboBox_Map"))
	TObjectPtr<UComboBoxString> ComboBox_Map;

	UPROPERTY(meta = (BindWidget = "ComboBox_Slots"))
	TObjectPtr<UComboBoxString>	ComboBox_Slots;

	UPROPERTY(meta = (BindWidget = "Button_Create"))
	TObjectPtr<UButton> Button_Create;

	UPROPERTY(meta = (BindWidget = "Button_Back"))
	TObjectPtr<UButton> Button_Back;

	UPROPERTY(Transient, meta = (BindWidgetAnim = "FloatingAnimation"))
	TObjectPtr<UWidgetAnimation> FloatingAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CreateSessionPopup", meta = (AllowPrivateAccess = "true"))
	FVector2D PopupSize;
};
