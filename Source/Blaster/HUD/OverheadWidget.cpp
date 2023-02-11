// Fill out your copyright notice in the Description page of Project Settings.


#include "OverheadWidget.h"
#include "Components/TextBlock.h"

void UOverheadWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	RemoveFromParent();
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}

void UOverheadWidget::SetDisplayText(FString TextDisplay)
{
	if (DisplayText)
	{
		DisplayText->SetText(FText::FromString(TextDisplay));
	}
}

void UOverheadWidget::SetPlayerName()
{
	if (DisplayText)
	{
		DisplayText->SetText(FText::FromString(GetOwningPlayerState()->GetPlayerName()));
	}
}