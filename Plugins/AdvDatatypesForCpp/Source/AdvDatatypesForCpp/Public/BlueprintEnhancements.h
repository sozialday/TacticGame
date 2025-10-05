// Intuitive Workflows | created by : Chopstig [Silvergull Studio] | Copyright 2025 | Unreal Engine 5.5
// Version 2025.1.0

#pragma once

#include"CoreMinimal.h"
#include "Engine.h"
#include "Datatypes.h"

#include "Kismet/BlueprintFunctionLibrary.h"
#include "BlueprintEnhancements.generated.h"

// simple structure for easier calculation with multiple three point structures [Vector, Rotator etc.]
struct FThreepointStruct
{
	double  
			x,
			y,
			z;

	FThreepointStruct() {	}
	
	FThreepointStruct(FVector InputVector)
	{
		x = InputVector.X;
		y = InputVector.Y;
		z = InputVector.Z;
	}
	FThreepointStruct(FRotator InputRotator)
	{
		x = InputRotator.Pitch;
		y = InputRotator.Yaw;
		z = InputRotator.Roll;
	}
	FThreepointStruct(FQuat InputQuaternion)
	{
		FThreepointStruct(FConv::ToRotator(InputQuaternion));
	}
};

UENUM()
enum EIgnoreModuloTransformType
{
	/* Uses everything for Modulo Calculation */
	All,

	/* Location and Rotation are used. Scale will be ignored for calculation */
	IgnoreScale,

	/* Location and Scale are used. Rotation will be ignored for calculation */
	IgnoreRotation,

	/* Rotation and Scale are used. Location will be ignored for calculation */
	IgnoreLocation,

	/* Only the Location Vector will be used for calculation */
	OnlyTransform,

	/* Only the Rotation Rotator will be used for calculation */
	OnlyRotation,

	/* Only the Scale Vector will be used for calculation */
	OnlyScale
};


/*
 *	Some Blueprint Workflow Enhancements 

	- includes arithmetical operations with variables that aren't supported by default

	e.g. : Rotator Addition / Subtraction etc.
 */
UCLASS()
class ADVDATATYPESFORCPP_API UBlueprintEnhancements : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintPure, meta = (DefaultToSelf = "Object", BlueprintThreadSafe, CompactNodeTitle = "GetWorld", Keywords = "Get World", DisplayName = "Get World"), Category = "World Context")
	static UWorld* GetWorld(UObject* Object)
	{
		return Object ? Object->GetWorld() : nullptr;
	}

		// Rotator Arithmetics [..]

	// Addition for Rotators
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe, CompactNodeTitle = "+", Keywords = "Addition +", DisplayName = "Addition [Rotator]", CommutativeAssociativeBinaryOperator = "true"), Category = "Operators")
	static FRotator Rotation_Add(FRotator A, FRotator B)
	{
		return A + B;
	}

	// Subtraction for Rotators
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe, CompactNodeTitle = "-", Keywords = "Subtraction -", DisplayName = "Subtraction [Rotator]", CommutativeAssociativeBinaryOperator = "true"), Category = "Operators")
	static FRotator Rotation_Sub(FRotator A, FRotator B)
	{
		return A - B;
	}

	// Multiplication for Rotators
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe, CompactNodeTitle = "*", Keywords = "Multiplication *", DisplayName = "Multiplication [Rotator]", CommutativeAssociativeBinaryOperator = "true"), Category = "Operators")
	static FRotator Rotation_Mult(FRotator A, FRotator B)
	{
		// simple multiplication
		return { A.Pitch * B.Pitch, A.Yaw * B.Yaw, A.Roll * B.Roll };
	}
	
	// Division for Rotators
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe, CompactNodeTitle = "/", Keywords = "Division /", DisplayName = "Division [Rotator]", CommutativeAssociativeBinaryOperator = "true"), Category = "Operators")
	static FRotator Rotation_Div(FRotator A, FRotator B)
	{
		// simple multiplication
		return { A.Pitch / B.Pitch, A.Yaw / B.Yaw, A.Roll / B.Roll };
	}

	// Modulo Calculation for Rotators
	// result of each elements modulo will be added together
	// * If B = 0 then it will return zero since division by 0 will probably nuke your PC
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe, CompactNodeTitle = "%", Keywords = "Modulo %", DisplayName = "Modulo [Rotator]"), Category = "Operators")
	static float Rotation_Mod(FRotator A, FRotator B)
	{
		// Modulo Calculation of the Rotator
		return Modulo(A, B);
	}
		
		// Transform Arithmetics [..]

	// Addition for Transforms
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe, CompactNodeTitle = "+", Keywords = "Addition +", DisplayName = "Addition [Transform]", CommutativeAssociativeBinaryOperator = "true"), Category = "Operators")
	static FTransform Transform_Add(FTransform A, FTransform B)
	{
		return A + B;
	}

	// Subtraction for Transforms
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe, CompactNodeTitle = "-", Keywords = "Subtraction -", DisplayName = "Subtraction [Transform]", CommutativeAssociativeBinaryOperator = "true"), Category = "Operators")
	static FTransform Transform_Sub(FTransform A, FTransform B)
	{
		return { FConv::ToQuaternion(Rotation_Sub(A.Rotator(), B.Rotator())), (A.GetLocation() - B.GetLocation()), (A.GetScale3D() - B.GetScale3D()) };
	}

	// Multiplication for Transforms
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe, CompactNodeTitle = "*", Keywords = "Multiplication *", DisplayName = "Multiplication [Transform]", CommutativeAssociativeBinaryOperator = "true"), Category = "Operators")
	static FTransform Transform_Mult(FTransform A, FTransform B)
	{
		return { FConv::ToQuaternion(Rotation_Mult(A.Rotator(), B.Rotator())), (A.GetLocation() * B.GetLocation()), (A.GetScale3D() * B.GetScale3D()) };
	}

	// Division for Transforms
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe, CompactNodeTitle = "/", Keywords = "Division /", DisplayName = "Division [Rotator]", CommutativeAssociativeBinaryOperator = "true"), Category = "Operators")
	static FTransform Transform_Div(FTransform A, FTransform B)
	{
		return { FConv::ToQuaternion(Rotation_Div(A.Rotator(), B.Rotator())), (A.GetLocation() / B.GetLocation()), (A.GetScale3D() / B.GetScale3D()) };
	}

	// Modulo Calculation for Transforms
	// result of each elements modulo will be added together
	// * If B = 0 then it will return zero since division by 0 will probably nuke your PC
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe, CompactNodeTitle = "%", Keywords = "Modulo %", DisplayName = "Modulo [Transform]"), Category = "Operators")
	static float Transform_Mod(FTransform A, FTransform B, TEnumAsByte<EIgnoreModuloTransformType> IgnoreCalculationType = All);


		// Text "Arithmetics"

	// Add [Append] Text
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe, Keywords = "Addition + Append", DisplayName = "Append [Text]", CommutativeAssociativeBinaryOperator = "true"), Category = "Operators|Append")
	static FText Text_Add(FText A, FText B);

		// Name "Arithmetics"

	// Add [Append] Name
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe, Keywords = "Addition + Append", DisplayName = "Append [Name]", CommutativeAssociativeBinaryOperator = "true"), Category = "Operators|Append")
	static FName Name_Add(FName A, FName B);

	
		// Literal Functions

	/**
	 * Creates a literal vector
	 * @param	InputVector	value to set the vector to
	 * @return	The literal vector
	 */
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe, Keywords = "MakeLiteral", DisplayName = "Make Literal Vector"), Category = "Math|Vector")
	static FVector MakeLiteralVector(FVector InputVector)
	{
		return InputVector;
	}
	/**
	 * Creates a literal rotator
	 * @param	InputVector	value to set the rotator to
	 * @return	The literal rotator
	 */
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe, Keywords = "MakeLiteral", DisplayName = "Make Literal Rotator"), Category = "Math|Rotator")
	static FRotator MakeLiteralRotator(FRotator InputVector)
	{
		return InputVector;
	}
	/**
	 * Creates a literal transform
	 * @param	InputVector		value to set the location to
	 * @param	InputRotation	value to set the rotation to
	 * @param	InputScale		value to set the scale to
	 * @return	The literal transform
	 */
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe, Keywords = "MakeLiteral", DisplayName = "Make Literal Transform"), Category = "Math|Transform")
	static FTransform MakeLiteralTransform(FVector InputVector, FRotator InputRotation, FVector InputScale)
	{
		return { InputRotation, InputVector, InputScale };
	}
	/**
	 * Creates a literal vector2d
	 * @param	InputVector2D	value to set the vector2d to
	 * @return	The literal vector2d
	 */
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe, Keywords = "MakeLiteral", DisplayName = "Make Literal Vector 2D"), Category = "Math|Vector")
	static FVector2D MakeLiteralVector2D(FVector2D InputVector2D)
	{
		return InputVector2D;
	}
	/**
	 * Creates a literal object reference
	 * @param	InputObject	value to set the object to
	 * @return	The literal object
	 */
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe, Keywords = "MakeLiteral", DisplayName = "Make Literal Object", DeterminesOutputType = "InputObject"), Category = "Utilities|Object")
	static UObject* MakeLiteralObject(TSubclassOf<UObject> InputObject)
	{
		return InputObject;
	}


	///**
	//*	Retrives a timed sine curve interpolating from 1 to -1 starting from 0
	//* 
	//* @return A smooth sinewave
	//*/
	//UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe, Keywords = "Time, Sine", DisplayName = "Time [Sine]", WorldContext = "WorldContext"), Category = "Math")
	//float GetTimedSineWave(UObject* WorldContext)
	//{
	//	return FFunc::TimedSine(WorldContext);
	//}



private:

	// since it is private and the only use is here asserts are not necessary
	static float Modulo(const FThreepointStruct& A, const FThreepointStruct& B)
	{
		// modulo calculation -> ensures that there is no division by 0
		return B.x != 0.0 ? FMath::Fmod(A.x, B.x) : 0.0 + B.y != 0.0 ? FMath::Fmod(A.y, B.y) : 0.0 + B.z != 0.0 ? FMath::Fmod(A.z, B.z) : 0.0;
	}
};
