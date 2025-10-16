// Fill out your copyright notice in the Description page of Project Settings.


#include "PerkFullscreenListBase.h"

#include "Components/GridPanel.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "TacticGame/UserInterface/InStage/GenericWidgets/SpacerWidget.h"

// Default Constructor
UPerkFullscreenListBase::UPerkFullscreenListBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	{
		// perk Card class
		static ConstructorHelpers::FClassFinder<UUserWidget> perkCardClass
		(TEXT("/Game/UserInterface/InStage/Overlays/Perks/WB_PerkCard"));

		if (perkCardClass.Succeeded())
			PerkCardWidgetClass = perkCardClass.Class;
	}

	{
		// spacer class
		static ConstructorHelpers::FClassFinder<USpacerWidget> spacerClass
		(TEXT("/Game/UserInterface/InStage/Overlays/Perks/Extras/Spacer"));

		if (spacerClass.Succeeded())
			SpacerWidgetClass = spacerClass.Class;
	}
}

void UPerkFullscreenListBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Amount = FMath::Clamp(Amount, 0, MaxAmount);
	CreateAndAddCardsToGrid();
	
}

// 14 cards : 1,330 units
// 13 spacers : 195 units
// combined : 1,525 units
void UPerkFullscreenListBase::CreateAndAddCardsToGrid()
{
	// 2 Rows
	for (int x = 0; x < 3; x++)
	{
		// 16 columns
		for (int y = 0; y < 27; y++)
		{
			if (CurrentAmount >= Amount)
				return;

			const auto& cardWidget = CreateWidget<UUserWidget>(GetWorld(), PerkCardWidgetClass);
			const auto& spacerWidgetX = CreateWidget<USpacerWidget>(GetWorld(), SpacerWidgetClass);
			spacerWidgetX->InitializeSpacerDimensions(FVector2D(15.0, 0.0));

			if (IsValid(cardWidget))
			{
				GetGridPanel()->AddChildToGrid(cardWidget, x, y);

				if (y == 26)
					continue;

				GetGridPanel()->AddChildToGrid(spacerWidgetX, x, y + 1);
				y++;
			}

			CurrentAmount++;
		}

		if (x == 2)
			break;

		const auto& spacerWidgetY = CreateWidget<USpacerWidget>(GetWorld(), SpacerWidgetClass);
		spacerWidgetY->InitializeSpacerDimensions(FVector2D(0.0, 10.0));
		
		GetGridPanel()->AddChildToGrid(spacerWidgetY, x+1);
		x++;
	}
}
