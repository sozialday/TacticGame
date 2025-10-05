// Intuitive Workflows | created by : Chopstig [Silvergull Studio] | Copyright 2025 | Unreal Engine 5.5
// Version 2025.1.0


#include "Datatypes.h"

	// TInt [FInt] - Functions

// Numerical conversion
FInt AdvDatatypesAndConversions::Arithmetics::IntegralType::TInt::FromNum(float InputValue)
{
	return InputValue;
}
// Numerical conversion
FInt AdvDatatypesAndConversions::Arithmetics::IntegralType::TInt::FromNum(double InputValue)
{
	return InputValue;
}

// Vector conversion
FInt AdvDatatypesAndConversions::Arithmetics::IntegralType::TInt::FromVector(FVector InputValue, EVectorUseType VectorUse)
{
	switch (VectorUse)
	{
	case x:
		return InputValue.X;
	case y:
		return InputValue.Y;
	case z:
		return InputValue.Z;
	}

	// has to be there.. compiler won't do it's job ;(
	return TInt();
}
// Vector conversion
FInt AdvDatatypesAndConversions::Arithmetics::IntegralType::TInt::FromVector(FVector2D InputValue, EVectorUseType VectorUse)
{
	switch (VectorUse)
	{
	case x:
		return InputValue.X;
	case y:
		return InputValue.Y;
	}

	// returns the default Integer [0] since the z case is not necessary
	return TInt();
}

// Rotator conversion
FInt AdvDatatypesAndConversions::Arithmetics::IntegralType::TInt::FromRotator(FRotator InputValue, ERotatorUseType RotatorUse)
{
	switch (RotatorUse)
	{
	case Pitch:
		return InputValue.Pitch;
	case Yaw:
		return InputValue.Yaw;
	case Roll:
		return InputValue.Roll;
	}

	// has to be there.. compiler won't do it's job ;(
	return TInt();
}

// String conversion
FInt AdvDatatypesAndConversions::Arithmetics::IntegralType::TInt::FromString(FString InputString, bool IgnoreNonNumericCharacters)
{
	// returns the default Integer [0] since the string has to be numeric 
	if (InputString.IsEmpty())
		return TInt();

	if (!IgnoreNonNumericCharacters)
	{
		if (InputString.IsNumeric())
			return FCString::Atoi(*InputString);
	}

	// simple conversion from string to Integer
	return FCString::Atoi64(*CutNonNumericCharacters(InputString));
}

// Boolean conversion
FInt AdvDatatypesAndConversions::Arithmetics::IntegralType::TInt::FromBool(bool InputValue)
{
	return InputValue;
}

// Byte conversion
FInt AdvDatatypesAndConversions::Arithmetics::IntegralType::TInt::FromByte(uint8 InputByte)
{
	return InputByte;
}

// String representation of the value
FString AdvDatatypesAndConversions::Arithmetics::IntegralType::TInt::ToString() const
{
	return FString::Printf(TEXT("%d"), Get());
}
// Text representation of the value
FText AdvDatatypesAndConversions::Arithmetics::IntegralType::TInt::ToText() const
{
	return FText::AsCultureInvariant(ToString());
}
// Name representation of the value
FName AdvDatatypesAndConversions::Arithmetics::IntegralType::TInt::ToName() const
{
	return FName(*ToString());
}

// retrieves an array containing each "element" of the integer
TArray<FString> AdvDatatypesAndConversions::Arithmetics::IntegralType::TInt::ToStringArray()
{
	// if the number is only one character long then the looping through the array is unecessary
	if (Get() < 10 && Get() > -10)
		return { ToString() };

	TArray<FString> NewStringArray;

	for (auto CharIt(ToString().CreateConstIterator()); CharIt; ++CharIt)
	{
		TCHAR Char = *CharIt;
		NewStringArray.Add(FString(1, &Char));
	}

	// clears the null terminator on the end of the string
	NewStringArray.RemoveAt(NewStringArray.Num() - 1, 1);
	return NewStringArray;
}

// Vector conversion of the integer
FVector AdvDatatypesAndConversions::Arithmetics::IntegralType::TInt::ToVector() const
{
	return FVector(m_Value);
}

// Rotator conversion of the integer
FRotator AdvDatatypesAndConversions::Arithmetics::IntegralType::TInt::ToRotator() const
{
	return FRotator(m_Value);
}


//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//

	// TFloat [FFloat] - Functions

// Numerical conversion
FFloat AdvDatatypesAndConversions::Arithmetics::FloatationType::TFloat::FromNum(int InputValue)
{
	return InputValue;
}
// Numerical conversion
FFloat AdvDatatypesAndConversions::Arithmetics::FloatationType::TFloat::FromNum(double InputValue)
{
	return InputValue;
}

// Boolean conversion
FFloat AdvDatatypesAndConversions::Arithmetics::FloatationType::TFloat::FromBool(bool InputValue)
{
	return InputValue;
}

// Vector conversion
FFloat AdvDatatypesAndConversions::Arithmetics::FloatationType::TFloat::FromVector(FVector InputValue, EVectorUseType VectorUse)
{
	switch (VectorUse)
	{
	case x:
		return InputValue.X;
	case y:
		return InputValue.Y;
	case z:
		return InputValue.Z;
	}

	// has to be there.. compiler won't do it's job ;(
	return FFloat();
}
// Vector conversion
FFloat AdvDatatypesAndConversions::Arithmetics::FloatationType::TFloat::FromVector(FVector2D InputValue, EVectorUseType VectorUse)
{
	switch (VectorUse)
	{
	case x:
		return InputValue.X;
	case y:
		return InputValue.Y;
	}

	// returns the default Integer [0] since the z case is not necessary
	return FFloat();
}

// Rotator conversion
FFloat AdvDatatypesAndConversions::Arithmetics::FloatationType::TFloat::FromRotator(FRotator InputValue, ERotatorUseType RotatorUse)
{
	switch (RotatorUse)
	{
	case Pitch:
		return InputValue.Pitch;
	case Yaw:
		return InputValue.Yaw;
	case Roll:
		return InputValue.Roll;
	}

	// has to be there.. compiler won't do it's job ;(
	return FFloat();
}

// String conversion
FFloat AdvDatatypesAndConversions::Arithmetics::FloatationType::TFloat::FromString(FString InputString, bool IgnoreNonNumericCharacters)
{
	// returns the default Integer [0] since the string has to be numeric 
	if (InputString.IsEmpty())
		return FFloat();

	if (!IgnoreNonNumericCharacters)
	{
		if (InputString.IsNumeric())
			return FCString::Atoi(*InputString);
	}

	// simple conversion from string to Integer
	return FCString::Atoi64(*CutNonNumericCharacters(InputString));
}

// Byte conversion
FFloat AdvDatatypesAndConversions::Arithmetics::FloatationType::TFloat::FromByte(uint8 InputByte)
{
	return InputByte;
}

// String representation of the value
FString AdvDatatypesAndConversions::Arithmetics::FloatationType::TFloat::ToString() const
{
	return FString::Printf(TEXT("%d"), Get());
}
// Text representation of the value
FText AdvDatatypesAndConversions::Arithmetics::FloatationType::TFloat::ToText() const
{
	return FText::AsCultureInvariant(ToString());
}
// Name representation of the value
FName AdvDatatypesAndConversions::Arithmetics::FloatationType::TFloat::ToName() const
{
	return FName(*ToString());
}

// retrieves an array containing each "element" of the integer
TArray<FString> AdvDatatypesAndConversions::Arithmetics::FloatationType::TFloat::ToStringArray()
{
	// if the number is only one character long then the looping through the array is unecessary
	if (Get() < 10 && Get() > -10)
		return { ToString() };

	TArray<FString> NewStringArray;

	for (auto CharIt(ToString().CreateConstIterator()); CharIt; ++CharIt)
	{
		TCHAR Char = *CharIt;
		NewStringArray.Add(FString(1, &Char));
	}

	// clears the null terminator on the end of the string
	NewStringArray.RemoveAt(NewStringArray.Num() - 1, 1);
	return NewStringArray;
}

// Vector conversion of the integer
FVector AdvDatatypesAndConversions::Arithmetics::FloatationType::TFloat::ToVector() const
{
	return FVector(m_Value);
}

// Rotator conversion of the integer
FRotator AdvDatatypesAndConversions::Arithmetics::FloatationType::TFloat::ToRotator() const
{
	return FRotator(m_Value);
}


//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//

	// TDouble [FDouble] - Functions

// Numerical conversion
FDouble AdvDatatypesAndConversions::Arithmetics::FloatationType::TDouble::FromNum(int InputValue)
{
	return InputValue;
}
// Numerical conversion
FDouble AdvDatatypesAndConversions::Arithmetics::FloatationType::TDouble::FromNum(float InputValue)
{
	return InputValue;
}

// Boolean conversion
FDouble AdvDatatypesAndConversions::Arithmetics::FloatationType::TDouble::FromBool(bool InputValue)
{
	return InputValue;
}

// Vector conversion
FDouble AdvDatatypesAndConversions::Arithmetics::FloatationType::TDouble::FromVector(FVector InputValue, EVectorUseType VectorUse)
{
	switch (VectorUse)
	{
	case x:
		return InputValue.X;
	case y:
		return InputValue.Y;
	case z:
		return InputValue.Z;
	}

	// has to be there.. compiler won't do it's job ;(
	return FDouble();
}
// Vector conversion
FDouble AdvDatatypesAndConversions::Arithmetics::FloatationType::TDouble::FromVector(FVector2D InputValue, EVectorUseType VectorUse)
{
	switch (VectorUse)
	{
	case x:
		return InputValue.X;
	case y:
		return InputValue.Y;
	}

	// returns the default Integer [0] since the z case is not necessary
	return FDouble();
}

// Rotator conversion
FDouble AdvDatatypesAndConversions::Arithmetics::FloatationType::TDouble::FromRotator(FRotator InputValue, ERotatorUseType RotatorUse)
{
	switch (RotatorUse)
	{
	case Pitch:
		return InputValue.Pitch;
	case Yaw:
		return InputValue.Yaw;
	case Roll:
		return InputValue.Roll;
	}

	// has to be there.. compiler won't do it's job ;(
	return FDouble();
}

// String conversion
FDouble AdvDatatypesAndConversions::Arithmetics::FloatationType::TDouble::FromString(FString InputString, bool IgnoreNonNumericCharacters)
{
	// returns the default Integer [0] since the string has to be numeric 
	if (InputString.IsEmpty())
		return FDouble();

	if (!IgnoreNonNumericCharacters)
	{
		if (InputString.IsNumeric())
			return FCString::Atoi(*InputString);
	}

	// simple conversion from string to Integer
	return FCString::Atoi64(*CutNonNumericCharacters(InputString));
}

// Byte conversion
FDouble AdvDatatypesAndConversions::Arithmetics::FloatationType::TDouble::FromByte(uint8 InputByte)
{
	return InputByte;
}

// String representation of the value
FString AdvDatatypesAndConversions::Arithmetics::FloatationType::TDouble::ToString() const
{
	return FString::Printf(TEXT("%d"), Get());
}
// Text representation of the value
FText AdvDatatypesAndConversions::Arithmetics::FloatationType::TDouble::ToText() const
{
	return FText::AsCultureInvariant(ToString());
}
// Name representation of the value
FName AdvDatatypesAndConversions::Arithmetics::FloatationType::TDouble::ToName() const
{
	return FName(*ToString());
}

// retrieves an array containing each "element" of the integer
TArray<FString> AdvDatatypesAndConversions::Arithmetics::FloatationType::TDouble::ToStringArray()
{
	// if the number is only one character long then the looping through the array is unecessary
	if (Get() < 10 && Get() > -10)
		return { ToString() };

	TArray<FString> NewStringArray;

	for (auto CharIt(ToString().CreateConstIterator()); CharIt; ++CharIt)
	{
		TCHAR Char = *CharIt;
		NewStringArray.Add(FString(1, &Char));
	}

	// clears the null terminator on the end of the string
	NewStringArray.RemoveAt(NewStringArray.Num() - 1, 1);
	return NewStringArray;
}

// Vector conversion of the integer
FVector AdvDatatypesAndConversions::Arithmetics::FloatationType::TDouble::ToVector() const
{
	return FVector(m_Value);
}

// Rotator conversion of the integer
FRotator AdvDatatypesAndConversions::Arithmetics::FloatationType::TDouble::ToRotator() const
{
	return FRotator(m_Value);
}


//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//

	// TBool [FBool] - Functions

// Numerical conversion
FBool AdvDatatypesAndConversions::Arithmetics::BooleanType::TBool::FromNum(int InputValue)
{
	return InputValue > 0 ? true : false;
}
// Numerical conversion
FBool AdvDatatypesAndConversions::Arithmetics::BooleanType::TBool::FromNum(double InputValue)
{
	return InputValue > 0 ? true : false;
}
// Numerical conversion
FBool AdvDatatypesAndConversions::Arithmetics::BooleanType::TBool::FromNum(float InputValue)
{
	return InputValue > 0 ? true : false;
}

// Vector conversion
FBool AdvDatatypesAndConversions::Arithmetics::BooleanType::TBool::FromVector(FVector InputValue, EVectorUseType VectorUse)
{
	switch (VectorUse)
	{
	case x:
		return FBool::FromNum(InputValue.X);
	case y:
		return FBool::FromNum(InputValue.Y);
	case z:
		return FBool::FromNum(InputValue.Z);
	}

	// has to be there.. compiler won't do it's job ;(
	return TBool();
}
// Vector conversion
FBool AdvDatatypesAndConversions::Arithmetics::BooleanType::TBool::FromVector(FVector2D InputValue, EVectorUseType VectorUse)
{
	switch (VectorUse)
	{
	case x:
		return FBool::FromNum(InputValue.X);
	case y:
		return FBool::FromNum(InputValue.Y);
	}

	// returns the default Integer [0] since the z case is not necessary
	return TBool();
}

// Rotator conversion
FBool AdvDatatypesAndConversions::Arithmetics::BooleanType::TBool::FromRotator(FRotator InputValue, ERotatorUseType RotatorUse)
{
	switch (RotatorUse)
	{
	case Pitch:
		return FBool::FromNum(InputValue.Pitch);
	case Yaw:
		return FBool::FromNum(InputValue.Yaw);
	case Roll:
		return FBool::FromNum(InputValue.Roll);
	}

	// has to be there.. compiler won't do it's job ;(
	return TBool();
}

// String conversion
FBool AdvDatatypesAndConversions::Arithmetics::BooleanType::TBool::FromString(FString InputString)
{
	// returns the default Integer [0] since the string has to be numeric 
	if (InputString.IsEmpty())
		return TBool();

	// contains only numbers 
	const FString NonNumericString = CutNonNumericCharacters(InputString);

	// converts the "string number" to a boolean
	return FCString::Atoi64(*NonNumericString) > 0 ? true : false;
}

// Byte conversion
FBool AdvDatatypesAndConversions::Arithmetics::BooleanType::TBool::FromByte(uint8 InputByte)
{
	return InputByte > 0 ? true : false;
}

// String representation of the value
FString AdvDatatypesAndConversions::Arithmetics::BooleanType::TBool::ToString(bool bUseNumericOutput) const
{
	return bUseNumericOutput ? FString::Printf(TEXT("%d"), Get()) : (Get() ? "true" : "false");
}
// Text representation of the value
FText AdvDatatypesAndConversions::Arithmetics::BooleanType::TBool::ToText() const
{
	return FText::AsCultureInvariant(ToString());
}
// Name representation of the value
FName AdvDatatypesAndConversions::Arithmetics::BooleanType::TBool::ToName() const
{
	return FName(*ToString());
}