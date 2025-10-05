// Intuitive Workflows | created by : Chopstig [Silvergull Studio] | Copyright 2025 | Unreal Engine 5.5
// Version 2025.1.0


#include "BlueprintEnhancements.h"

// Transform Modulo
float UBlueprintEnhancements::Transform_Mod(FTransform A, FTransform B, TEnumAsByte<EIgnoreModuloTransformType> IgnoreCalculationType)
{
	switch (IgnoreCalculationType)
	{
	case All:				/* Uses everything for Modulo Calculation */
		return Modulo(A.GetLocation(), B.GetLocation()) + Modulo(A.GetRotation(), B.GetRotation()) + Modulo(A.GetScale3D(), B.GetScale3D());

	case IgnoreScale: 		/* Location and Rotation are used. Scale will be ignored for calculation */

		return Modulo(A.GetLocation(), B.GetLocation()) + Modulo(A.GetRotation(), B.GetRotation());

	case IgnoreRotation:	/* Location and Scale are used. Rotation will be ignored for calculation */
	
		return Modulo(A.GetLocation(), B.GetLocation()) + Modulo(A.GetScale3D(), B.GetScale3D());

	case IgnoreLocation:	/* Rotation and Scale are used. Location will be ignored for calculation */

		return Modulo(A.GetRotation(), B.GetRotation()) + Modulo(A.GetScale3D(), B.GetScale3D());

	case OnlyTransform:		/* Only the Location Vector will be used for calculation */

		return Modulo(A.GetLocation(), B.GetLocation());

	case OnlyRotation:		/* Only the Rotation Rotator will be used for calculation */

		return Modulo(A.GetRotation(), B.GetRotation());

	case OnlyScale:			/* Only the Scale Vector will be used for calculation */

		return Modulo(A.GetScale3D(), B.GetScale3D());
	}

	// when [somehow] an enum has not been selected
	return 0.0f;
}

// Append Text
FText UBlueprintEnhancements::Text_Add(FText A, FText B)
{
	return FConv::ToText(FConv::ToString(A) + FConv::ToString(B));
}
// Append Name
FName UBlueprintEnhancements::Name_Add(FName A, FName B)
{
	return FConv::ToName(FConv::ToString(A) + FConv::ToString(B));
}