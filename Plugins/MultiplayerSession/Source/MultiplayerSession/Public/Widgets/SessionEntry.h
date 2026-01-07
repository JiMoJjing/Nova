// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "SessionEntry.generated.h"

class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSION_API USessionEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	USessionEntry(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

private:
	UFUNCTION()
	void JoinButtonReleased();

private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget = "TextBlock_SessionName", AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> TextBlock_SessionName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget = "TextBlock_MapName", AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> TextBlock_MapName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget = "TextBlock_UserName", AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> TextBlock_UserName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget = "TextBlock_PlayersNum", AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> TextBlock_PlayersNum;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget = "TextBlock_Ping", AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> TextBlock_Ping;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget = "Button_Join", AllowPrivateAccess = "true"))
	TObjectPtr<UButton> Button_Join;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 SessionSearchResultIndex;
};
