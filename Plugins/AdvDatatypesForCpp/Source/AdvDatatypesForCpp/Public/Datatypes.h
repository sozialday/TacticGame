// Intuitive Workflows | created by : Chopstig [Silvergull Studio] | Copyright 2025 | Unreal Engine 5.5
// Version 2025.1.0

#pragma once

#include "functional"
#include "CoreMinimal.h"
#include "GeneralTypes.h" // contains the enums for the vector/rotator conversions

#include "Kismet/GameplayStatics.h"

// standard macros for easier development here ;)
	// private:
#define ensureAssert(TemplateName, AssertType, OutputMessage)		static_assert(AssertType<TemplateName>::value, OutputMessage)
#define ensureArithmetic(TemplateName)								static_assert(std::is_arithmetic<TemplateName>::value, "Input Type must be of arithmetic type")

// create a world context reference
#define ensureWorld(WorldContext) TObjectPtr<UWorld> GetWorld = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull); if (!GetWorld) return

	// easier Logging macros

// Print to the Unreal Engine Console [Output Log]
#define LogTo(Verbosity, Message)			UE_LOG(LogTemp, Verbosity, TEXT(Message))
// Print to the Viewport
#define Print(Time, Colour, Message)	if(GEngine) GEngine->AddOnScreenDebugMessage(-1, Time, Colour, Message)

//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//

// Functions and Dataypes for easier use in your C++ Workflow 😜
namespace AdvDatatypesAndConversions
{ 
	// The new Datatypes [..]
	namespace Arithmetics
	{
		// all structures which are of type integer [..]
		namespace IntegralType
		{
			//	//	//	Integer	//	//	//

			/*
			*
			*	A simple container for the integer data type that takes numeric values from -2^31 to (2^31 -1).
			*	It takes 4 Bytes (i.e., 32 bits) in the memory.
			*
			*	Additions to this class :
			*
			*	>> various conversion to this and from this class
			*
			*
			*	https://www.tutorialspoint.com/cplusplus/cpp_numeric_data_types.htm
			*		for more information regarding the integer datatype
			*/
			struct ADVDATATYPESFORCPP_API TInt
			{
				// Auto converter to the integer datatype
				operator int()
				{
					return m_Value;
				}

				// arithmetic operators [..]

				/*
				*	Adds another TInt to this.
				*
				*	@param B The other Integer.
				*	@return Copy of Integer after addition.
				*/
				TInt operator+=(const TInt& B)
				{
					m_Value += B.Get();
					return *this;
				}

				/*
				*	Subtracts another TInt to this.
				*
				*	@param B The other Integer.
				*	@return Copy of Integer after subtraction.
				*/
				TInt operator-=(const TInt& B)
				{
					m_Value -= B.Get();
					return *this;
				}

				/*
				*	Multiplies another TInt to this.
				*
				*	@param B The other Integer.
				*	@return Copy of Integer after multiplication.
				*/
				TInt operator*=(const TInt& B)
				{
					m_Value *= B.Get();
					return *this;
				}

				/*
				*	Divides another TInt to this.
				*
				*	@param B The other Integer.
				*	@return Copy of Integer after division.
				*/
				TInt operator/=(const TInt& B)
				{
					m_Value /= B.Get();
					return *this;
				}


				// Default Constructor with no parameters
				// will set to -1
				TInt()
					: m_Value(-1) { }

				// Constructor with an integer as input
				TInt(int Value)
					: m_Value(Value) { }


				// Converts a float value to an Integer
				static TInt FromNum(float InputValue = 0.0f);
				// Converts a double value to an Integer
				static TInt FromNum(double InputValue = 0.0);

				// Converts a bool to an Integer
				static TInt FromBool(bool InputValue = false);

				// Converts a Vector2D to an Integer by taking the x value
				// can be changed to use y or z
				static TInt FromVector(FVector InputValue = FVector(0), EVectorUseType VectorUse = EVectorUseType::x);
				// Converts a Vector2D to an Integer by taking the x value
				// can be changed to use y or z
				static TInt FromVector(FVector2D InputValue = FVector2D(0), EVectorUseType VectorUse = EVectorUseType::x);

				// Converts a Rotator to an Integer by taking the x axis
				// can be changed to use the y or z axis
				static TInt FromRotator(FRotator InputValue = FRotator(0), ERotatorUseType RotatorUse = ERotatorUseType::Pitch);

				// Converts a String to an Integer
				static TInt FromString(FString InputString, bool IgnoreNonNumericCharacters = true);
				// Converts a Text to an Integer
				static TInt FromText(FText InputText, bool IgnoreNonNumericCharacters = true)
				{
					// convert the text to a string and just do the String converison
					return FromString(InputText.ToString(), IgnoreNonNumericCharacters);
				}
				// Converts a Name to an Integer
				static TInt FromName(FName InputName, bool IgnoreNonNumericCharacters = true)
				{
					// convert the name to a string and just do the String converison
					return FromString(InputName.ToString(), IgnoreNonNumericCharacters);
				}

				// Converts a byte to an Integer
				static TInt FromByte(uint8 InputByte);

				// returns a string representation of the value currently stored
				FString ToString() const;
				// returns a text representation of the value currently stored
				FText ToText() const;
				// returns a name representation of the value currently stored
				FName ToName() const;

				// templated to array conversion where you can select the type you want to have in the array
				// 
				// Allowed datatypes : int ✓, float ✓, double ✓, long ✓, FString ☓
				template <typename T = int>
				TArray<T> ToNumArray();

				// converts the integer to a string array
				TArray<FString> ToStringArray();


				// templated conversion to any numeric type
				// 
				// Allowed datatypes	: int ✓, float ✓, double ✓, long ✓ etc.
				// Forbidden datatypes	: everything non numeric and nonprimitive  ☓
				// 
				// Syntax for proper use => .ToNum<short>();
				template <typename T = int>
				T ToNum();

				// returns a vector, where each element is that integer
				FVector ToVector() const;

				// returns a rotator, where each element is that integer
				FRotator ToRotator() const;

					// Boolean Member Functions [..]

				// is value negative
				bool IsNegative() const
				{
					return Get() < 0;
				}

				// is value equal to 0
				bool IsZero() const
				{
					return Get() == 0;
				}

				// is equal to number 
				template <typename T = TInt>
				bool IsEqualTo(const T& B) const;


					// Void Member Functions [..]

				// set integer to zero
				void ToZero()
				{
					m_Value = 0;
				}

			private:

				// retrieves the value stored inside this class
				int Get() const
				{
					return m_Value;
				}

				// Will cut the string making it contain only numbers
				static FString CutNonNumericCharacters(FString Input)
				{
					// Empty String
					FString NewString;

					if (!Input.IsEmpty())
					{
						for (auto CharIt(Input.CreateConstIterator()); CharIt; ++CharIt)
						{
							TCHAR Char = *CharIt;
							//	//	//
							FString StringSingle = FString(1, &Char);
							if (StringSingle.IsNumeric())
							{
								NewString += StringSingle;
							}
						}

						return NewString;
					}
					else return Input;
				}

			private:

				// The Value stored in this class
				int m_Value;
			};

			// conversion to templated array containing each element of the number stored
			template<typename T>
			FORCEINLINE TArray<T> TInt::ToNumArray()
			{
				// assert for when the conversion type is not of numeric type
				static_assert(std::is_arithmetic<T>::value || std::is_same<T, TInt>::value, "Template parameter must be of numeric type");

				// if the number is only one character long then the looping through the array is unecessary
				if (Get() < 10 && Get() > -10)
					return { static_cast<T>(Get()) };

				// the string representation of the number stored in this class
				const FString NumToString = ToString();

				TArray<T> ReturnArray;
				//	//	//
				for (auto CharIt(NumToString.CreateConstIterator()); CharIt; ++CharIt)
				{
					TCHAR Char = *CharIt;
					const FString Character = FString(1, &Char);
					//	//	//
					ReturnArray.Add(TInt::FromString(Character, false));
				}

				// Removes the last element from the array
				ReturnArray.RemoveAt(ReturnArray.Num() - 1, 1);

				return ReturnArray;
			}

			// conversion to templated numeric type
			template<typename T>
			FORCEINLINE T TInt::ToNum()
			{
				// assert for when the conversion type is not of numeric type
				static_assert(std::is_arithmetic<T>::value || std::is_same<T, TInt>::value, "Template parameter must be of numeric type");			// actual conversion
				return (T)m_Value;
			}

			// is value equal to a templated numeric value
			template<typename T>
			FORCEINLINE bool TInt::IsEqualTo(const T& B) const
			{
				// assert for when the conversion type is not of numeric type
				static_assert(std::is_arithmetic<T>::value || std::is_same<T, TInt>::value, "Template parameter must be of numeric type");

				// is value equal to parameter
				return Get() == B;
			}
		}

		// all structures which are of type "float" [..]
		namespace FloatationType
		{
			//	//	//	Float	//	//	//

			/*
			*
			*	A simple structure for the floating point data type that takes numbers that are followed by a decimal part.
			*	This data type takes 4 Bytes (i.e., 32 bits) of memory.
			*
			*
			*	Additions to this class :
			*
			*	>> various conversion to this and from this class
			*
			*
			*	https://www.tutorialspoint.com/cplusplus/cpp_numeric_data_types.htm
			*		for more information regarding the floating point datatype
			*/
			struct ADVDATATYPESFORCPP_API TFloat
			{
				// Auto converter to the float datatype
				operator float()
				{
					return m_Value;
				}

				// arithmetic operators [..]

				/*
				*	Adds another TFloat to this.
				*
				*	@param B The other Float.
				*	@return Copy of Float after addition.
				*/
				TFloat operator+=(const TFloat& B)
				{
					m_Value += B.Get();
					return *this;
				}

				/*
				*	Subtracts another TFloat to this.
				*
				*	@param B The other Float.
				*	@return Copy of Float after subtraction.
				*/
				TFloat operator-=(const TFloat& B)
				{
					m_Value -= B.Get();
					return *this;
				}

				/*
				*	Multiplies another TFloat to this.
				*
				*	@param B The other Float.
				*	@return Copy of Float after multiplication.
				*/
				TFloat operator*=(const TFloat& B)
				{
					m_Value *= B.Get();
					return *this;
				}

				/*
				*	Divides another TFloat to this.
				*
				*	@param B The other Float.
				*	@return Copy of Float after division.
				*/
				TFloat operator/=(const TFloat& B)
				{
					m_Value /= B.Get();
					return *this;
				}


				// Default Constructor with no parameters
				// will set to -1.0f
				TFloat()
					: m_Value(-1.0f) { }

				// Constructor with a float as input
				TFloat(float Value)
					: m_Value(Value) { }


				// Converts an integer to a float
				static TFloat FromNum(int InputValue = 0.0f);
				// Converts a double value to a float
				static TFloat FromNum(double InputValue = 0.0);

				// Converts a bool to a float
				static TFloat FromBool(bool InputValue = false);

				// Converts a Vector2D to a float by taking the x value
				// can be changed to use y or z
				static TFloat FromVector(FVector InputValue = FVector(0), EVectorUseType VectorUse = EVectorUseType::x);
				// Converts a Vector2D to a float by taking the x value
				// can be changed to use y or z
				static TFloat FromVector(FVector2D InputValue = FVector2D(0), EVectorUseType VectorUse = EVectorUseType::x);

				// Converts a Rotator to a float by taking the x axis
				// can be changed to use the y or z axis
				static TFloat FromRotator(FRotator InputValue = FRotator(0), ERotatorUseType RotatorUse = ERotatorUseType::Pitch);

				// Converts a String to a float
				static TFloat FromString(FString InputString, bool IgnoreNonNumericCharacters = true);
				// Converts a Text to a float
				static TFloat FromText(FText InputText, bool IgnoreNonNumericCharacters = true)
				{
					// convert the text to a string and just do the String converison
					return FromString(InputText.ToString(), IgnoreNonNumericCharacters);
				}
				// Converts a Name to a float
				static TFloat FromName(FName InputName, bool IgnoreNonNumericCharacters = true)
				{
					// convert the name to a string and just do the String converison
					return FromString(InputName.ToString(), IgnoreNonNumericCharacters);
				}

				// Converts a byte to a float
				static TFloat FromByte(uint8 InputByte);

				// returns a string representation of the value currently stored
				FString ToString() const;
				// returns a text representation of the value currently stored
				FText ToText() const;
				// returns a name representation of the value currently stored
				FName ToName() const;

				// templated to array conversion where you can select the type you want to have in the array
				// 
				// Allowed datatypes : int ✓, float ✓, double ✓, long ✓, FString ☓
				template <typename T = int>
				TArray<T> ToNumArray();

				// converts the float to a string array
				TArray<FString> ToStringArray();


				// templated conversion to any numeric type
				// 
				// Allowed datatypes	: int ✓, float ✓, double ✓, long ✓ etc.
				// Forbidden datatypes	: everything non numeric and nonprimitive  ☓
				// 
				// Syntax for proper use => .ToNum<short>();
				template <typename T = int>
				T ToNum();

				// returns a vector, where each element is that integer
				FVector ToVector() const;

				// returns a rotator, where each element is that integer
				FRotator ToRotator() const;


					// Boolean Member Functions [..]

				// is value negative
				bool IsNegative() const
				{
					return Get() < 0;
				}

				// is value equal to 0
				bool IsZero(float Tolerance = 1.e-8f) const
				{
					return FMath::IsNearlyZero(Get(), 1.e-8f);
				}

				// is equal to number 
				template <typename T = TFloat>
				bool IsEqualTo(const T& B, float Tolerance = 1.e-8f) const;


					// Void Member Functions [..]

				// set Float to zero
				void ToZero()
				{
					m_Value = 0.0f;
				}

			private:

				// retrieves the value stored inside this class
				int Get() const
				{
					return m_Value;
				}

				// Will cut the string making it contain only numbers
				static FString CutNonNumericCharacters(FString Input)
				{
					// Empty String
					FString NewString;

					if (!Input.IsEmpty())
					{
						for (auto CharIt(Input.CreateConstIterator()); CharIt; ++CharIt)
						{
							TCHAR Char = *CharIt;
							//	//	//
							FString StringSingle = FString(1, &Char);
							if (StringSingle.IsNumeric())
							{
								NewString += StringSingle;
							}
						}

						return NewString;
					}
					else return Input;
				}

				// The Value stored in this class
				float m_Value;
			};

			template<typename T>
			FORCEINLINE TArray<T> TFloat::ToNumArray()
			{
				// assert for when the conversion type is not of floating point type
				static_assert(std::is_floating_point<T>::value || std::is_same<T, TFloat>::value, "Template parameter must be of numeric type");

				// if the number is only one character long then the looping through the array is unecessary
				if (Get() < 10 && Get() > -10)
					return { static_cast<T>(Get()) };

				// the string representation of the number stored in this class
				const FString NumToString = ToString();

				TArray<T> ReturnArray;
				//	//	//
				for (auto CharIt(NumToString.CreateConstIterator()); CharIt; ++CharIt)
				{
					TCHAR Char = *CharIt;
					const FString Character = FString(1, &Char);
					//	//	//
					ReturnArray.Add(AdvDatatypesAndConversions::Arithmetics::IntegralType::TInt::FromString(Character, false));
				}

				// Removes the last element from the array
				ReturnArray.RemoveAt(ReturnArray.Num() - 1, 1);

				return ReturnArray;
			}

			template<typename T>
			FORCEINLINE T TFloat::ToNum()
			{
				// assert for when the conversion type is not of floating point type
				static_assert(std::is_floating_point<T>::value || std::is_same<T, TFloat>::value, "Template parameter must be of numeric type");

				// actual conversion
				return (T)m_Value;
			}

			template<typename T>
			FORCEINLINE bool TFloat::IsEqualTo(const T& B, float Tolerance) const
			{
				// assert for when the conversion type is not of floating point type
				static_assert(std::is_floating_point<T>::value || std::is_same<T, TFloat>::value, "Template parameter must be of numeric type");

				return FMath::IsNearlyEqual(Get(), B, Tolerance);
			}

			//	//	//	Double	//	//	//

			/*
			*
			*	A simple structure for the double data type that is used to store floating-point elements with double precision as compared to float.
			*	This data type takes 8 Bytes (i.e., 64 bits) of memory.
			*
			*
			*	Additions to this class :
			*
			*	>> various conversion to this and from this class
			*
			*
			*	https://www.tutorialspoint.com/cplusplus/cpp_numeric_data_types.htm
			*		for more information regarding the floating point datatype
			*/
			struct ADVDATATYPESFORCPP_API TDouble
			{
				// Auto converter to the double datatype
				operator double()
				{
					return m_Value;
				}

				// arithmetic operators [..]

				/*
				*	Adds another TDouble to this.
				*
				*	@param B The other Double.
				*	@return Copy of Double after addition.
				*/
				TDouble operator+=(const TDouble& B)
				{
					m_Value += B.Get();
					return *this;
				}

				/*
				*	Subtracts another TDouble to this.
				*
				*	@param B The other Double.
				*	@return Copy of Double after subtraction.
				*/
				TDouble operator-=(const TDouble& B)
				{
					m_Value -= B.Get();
					return *this;
				}

				/*
				*	Multiplies another TDouble to this.
				*
				*	@param B The other Double.
				*	@return Copy of Double after multiplication.
				*/
				TDouble operator*=(const TDouble& B)
				{
					m_Value *= B.Get();
					return *this;
				}

				/*
				*	Divides another TDouble to this.
				*
				*	@param B The other Double.
				*	@return Copy of Double after division.
				*/
				TDouble operator/=(const TDouble& B)
				{
					m_Value /= B.Get();
					return *this;
				}


				// Default Constructor with no parameters
				// will set to -1
				TDouble()
					: m_Value(-1.0) { }

				// Constructor with a double as input
				TDouble(double Value)
					: m_Value(Value) { }


				// Converts an integer value to a double
				static TDouble FromNum(int InputValue = 0.0f);
				// Converts a float value to a double
				static TDouble FromNum(float InputValue = 0.0);

				// Converts a bool to a double
				static TDouble FromBool(bool InputValue = false);

				// Converts a Vector2D to a double by taking the x value
				// can be changed to use y or z
				static TDouble FromVector(FVector InputValue = FVector(0), EVectorUseType VectorUse = EVectorUseType::x);
				// Converts a Vector2D to a double by taking the x value
				// can be changed to use y or z
				static TDouble FromVector(FVector2D InputValue = FVector2D(0), EVectorUseType VectorUse = EVectorUseType::x);

				// Converts a Rotator to a double by taking the x axis
				// can be changed to use the y or z axis
				static TDouble FromRotator(FRotator InputValue = FRotator(0), ERotatorUseType RotatorUse = ERotatorUseType::Pitch);

				// Converts a String to a double
				static TDouble FromString(FString InputString, bool IgnoreNonNumericCharacters = true);
				// Converts a Text to a double
				static TDouble FromText(FText InputText, bool IgnoreNonNumericCharacters = true)
				{
					// convert the text to a string and just do the String converison
					return FromString(InputText.ToString(), IgnoreNonNumericCharacters);
				}
				// Converts a Name to a double
				static TDouble FromName(FName InputName, bool IgnoreNonNumericCharacters = true)
				{
					// convert the name to a string and just do the String converison
					return FromString(InputName.ToString(), IgnoreNonNumericCharacters);
				}

				// Converts a byte to a double
				static TDouble FromByte(uint8 InputByte);

				// returns a string representation of the value currently stored
				FString ToString() const;
				// returns a text representation of the value currently stored
				FText ToText() const;
				// returns a name representation of the value currently stored
				FName ToName() const;

				// templated to array conversion where you can select the type you want to have in the array
				// 
				// Allowed datatypes : int ✓, float ✓, double ✓, long ✓, FString ☓
				template <typename T = int>
				TArray<T> ToNumArray();

				// converts the double to a string array
				TArray<FString> ToStringArray();


				// templated conversion to any numeric type
				// 
				// Allowed datatypes	: int ✓, float ✓, double ✓, long ✓ etc.
				// Forbidden datatypes	: everything non numeric and nonprimitive  ☓
				// 
				// Syntax for proper use => .ToNum<short>();
				template <typename T = int>
				T ToNum();

				// returns a vector, where each element is that integer
				FVector ToVector() const;

				// returns a rotator, where each element is that integer
				FRotator ToRotator() const;


				// Boolean Member Functions [..]

				// is value negative
				bool IsNegative() const
				{
					return Get() < 0;
				}

				// is value equal to 0
				bool IsZero(float Tolerance = 1.e-8f) const
				{
					return FMath::IsNearlyZero(Get(), 1.e-8f);
				}

				// is equal to number 
				template <typename T = TDouble>
				bool IsEqualTo(const T& B, float Tolerance = 1.e-8f) const;


					// Void Member Functions [..]

				// set Float to zero
				void ToZero()
				{
					m_Value = 0.0f;
				}

			private:

				// retrieves the value stored inside this class
				int Get() const
				{
					return m_Value;
				}

				// Will cut the string making it contain only numbers
				static FString CutNonNumericCharacters(FString Input)
				{
					// Empty String
					FString NewString;

					if (!Input.IsEmpty())
					{
						for (auto CharIt(Input.CreateConstIterator()); CharIt; ++CharIt)
						{
							TCHAR Char = *CharIt;
							//	//	//
							FString StringSingle = FString(1, &Char);
							if (StringSingle.IsNumeric())
							{
								NewString += StringSingle;
							}
						}

						return NewString;
					}
					else return Input;
				}

			private:

				// The Value stored in this class
				double m_Value;
			};

			template<typename T>
			FORCEINLINE TArray<T> TDouble::ToNumArray()
			{
				// assert for when the conversion type is not of floating point type
				static_assert(std::is_floating_point<T>::value || std::is_same<T, TDouble>::value, "Template parameter must be of numeric type");

				// if the number is only one character long then the looping through the array is unecessary
				if (Get() < 10 && Get() > -10)
					return { static_cast<T>(Get()) };

				// the string representation of the number stored in this class
				const FString NumToString = ToString();

				TArray<T> ReturnArray;
				//	//	//
				for (auto CharIt(NumToString.CreateConstIterator()); CharIt; ++CharIt)
				{
					TCHAR Char = *CharIt;
					const FString Character = FString(1, &Char);
					//	//	//
					ReturnArray.Add(AdvDatatypesAndConversions::Arithmetics::IntegralType::TInt::FromString(Character, false));
				}

				// Removes the last element from the array
				ReturnArray.RemoveAt(ReturnArray.Num() - 1, 1);

				return ReturnArray;
			}

			template<typename T>
			FORCEINLINE T TDouble::ToNum()
			{
				// assert for when the conversion type is not of floating point type
				static_assert(std::is_floating_point<T>::value || std::is_same<T, TDouble>::value, "Template parameter must be of numeric type");

				// actual conversion
				return (T)m_Value;
			}

			template<typename T>
			FORCEINLINE bool TDouble::IsEqualTo(const T& B, float Tolerance) const
			{
				// assert for when the conversion type is not of floating point type
				static_assert(std::is_floating_point<T>::value || std::is_same<T, TDouble>::value, "Template parameter must be of numeric type");

				return FMath::IsNearlyEqual(Get(), B, Tolerance);
			}
		}

		// all structures which are of the boolean type
		namespace BooleanType
		{
			//	//	// Boolean	//	//	//

			/*
			*
			*	A simple structure for the boolean data type which can be set to True or False.
			*
			*
			*	Additions to this class :
			*
			*	>> various conversion to this and from this class
			*
			*
			*	https://www.tutorialspoint.com/cplusplus/cpp_numeric_data_types.htm
			*		for more information regarding the floating point datatype
			*/
			struct ADVDATATYPESFORCPP_API TBool
			{
				// Auto converter to the integer datatype
				operator bool()
				{
					return m_Value;
				}


				// Default Constructor with no parameters
				// will set to -1.0f
				TBool()
					: m_Value(false) { }

				// Constructor with a boolean as input
				TBool(bool Value)
					: m_Value(Value) { }

				// Flips the Boolean
				void Flip()
				{
					m_Value = m_Value ? false : true;
				}

				// Flips the Boolean
				static void Flip(bool& TargetBoolean)
				{
					TargetBoolean = TargetBoolean ? false : true;
				}

				// flips the bool without altering the actual value
				TBool FlipRemote()
				{
					return Get() ? false : true;
				}

				// Converts an integer to a boolean
				static TBool FromNum(int InputValue = 0.0f);
				// Converts a double to a boolean
				static TBool FromNum(double InputValue = 0.0);
				// Converts an integer to a boolean
				static TBool FromNum(float InputValue = 0.0);

				// Converts a Vector2D to a boolean by taking the x value
				// can be changed to use y or z
				static TBool FromVector(FVector InputValue = FVector(0), EVectorUseType VectorUse = EVectorUseType::x);
				// Converts a Vector2D to a boolean by taking the x value
				// can be changed to use y or z
				static TBool FromVector(FVector2D InputValue = FVector2D(0), EVectorUseType VectorUse = EVectorUseType::x);

				// Converts a Rotator to a boolean by taking the x axis
				// can be changed to use the y or z axis
				static TBool FromRotator(FRotator InputValue = FRotator(0), ERotatorUseType RotatorUse = ERotatorUseType::Pitch);

				/*
				*	Converts a String to a boolean
				*
				*	This is a slow operation in comparison
				*
				*	@param IgnoreNonNumericCharacters :	If true numbers will be ignored and only letters will be used for conversion
				*/
				static TBool FromString(FString InputString);
				/*
				*	Converts a Text to a boolean
				*
				*	This is a slow operation in comparison
				*
				*	@param IgnoreNonNumericCharacters :	If true numbers will be ignored and only letters will be used for conversion
				*/
				static TBool FromText(FText InputText)
				{
					// convert the text to a string and just do the String converison
					return FromString(InputText.ToString());
				}
				/*
				*	Converts a Name to a boolean
				*
				*	This is a slow operation in comparison
				*
				*	@param IgnoreNonNumericCharacters :	If true numbers will be ignored and only letters will be used for conversion
				*/
				static TBool FromName(FName InputName)
				{
					// convert the name to a string and just do the String converison
					return FromString(InputName.ToString());
				}

				// Converts a byte to a boolean
				static TBool FromByte(uint8 InputByte);

				// returns a string representation of the value currently stored
				FString ToString(bool bUseNumericOutput = false) const;
				// returns a text representation of the value currently stored
				FText ToText() const;
				// returns a name representation of the value currently stored

				FName ToName() const;

				// templated conversion to any numeric type
				// 
				// Allowed datatypes	: int ✓, float ✓, double ✓, long ✓, bool ✓ etc.
				// Forbidden datatypes	: everything non numeric and nonprimitive  ☓
				// 
				// Syntax for proper use => .ToNum<float>();
				template <typename T = int>
				T ToNum();

			private:

				// retrieves the value stored inside this class
				bool Get() const
				{
					return m_Value;
				}

				// Will cut the string making it contain only numbers
				static FString CutNonNumericCharacters(FString Input)
				{
					// Empty String
					FString NewString;

					if (!Input.IsEmpty())
					{
						for (auto CharIt(Input.CreateConstIterator()); CharIt; ++CharIt)
						{
							TCHAR Char = *CharIt;
							//	//	//
							FString StringSingle = FString(1, &Char);
							if (StringSingle.IsNumeric())
							{
								NewString += StringSingle;
							}
						}

						return NewString;
					}
					else return Input;
				}

				// the primitive value
				bool m_Value;
			};

			template<typename T>
			FORCEINLINE T TBool::ToNum()
			{
				// assert for when the conversion type is not of floating point type
				static_assert(std::is_arithmetic<T>::value || std::is_same<T, TBool>::value, "Template parameter must be of numeric type");

				// actual conversion
				return m_Value;
			}
		}
	}

	// Several conversions
	namespace ExtraConversions
	{
		/*
		*	This class contains several other conversion methods that can be used to further enhance your coding experience
		*
		*	-> contains conversions through several classes and structs
		*/
		struct ADVDATATYPESFORCPP_API TGenericConversions
		{
			// Flips the Boolean
			static bool FlipBool(bool CurrentBool)
			{
				return CurrentBool ? false : true;
			}

			// Converts a LinearColour to the regular Colour maintaining quality as good as possible
			static FColor LinearColorToColor(const FLinearColor& InputLinearColour, bool bUseSRGB = true)
			{
				return InputLinearColour.ToFColor(bUseSRGB);
			}

			// Converts a Vector2D to an IntPoint
			static FIntPoint Vec2dToIntPoint(const FVector2D& InputVector)
			{
				return FIntPoint(InputVector.X, InputVector.Y);
			}

			// Converts an Integer to a IntVector by filling all values with that Integer
			static FIntVector IntToIntVector(const int& InputValue)
			{
				return { InputValue , InputValue , InputValue };
			}

			// Converts a Colour to a LinearColour
			static FLinearColor ColorToLinearColor(const FColor& InputColour)
			{
				return { InputColour };
			}

			// Converts a Rotator to a Quaternion
			static FQuat ToQuaternion(const FRotator& InputValue)
			{
				return InputValue.Quaternion();
			}
			// Converts a Vector to a Quaternion
			static FQuat ToQuaternion(const FVector& InputValue)
			{
				return InputValue.ToOrientationQuat();
			}
			// Converts a Vector4 to a Quaternion
			static FQuat ToQuaternion(const FVector4& InputValue)
			{
				return InputValue.ToOrientationQuat();
			}

			// Converts a Vector4 to a Rotator
			static FRotator ToRotator(const FVector4& InputValue)
			{
				return InputValue.ToOrientationRotator();
			}
			// Converts a Matrix to a Rotator
			static FRotator ToRotator(const FMatrix& InputValue)
			{
				return InputValue.Rotator();
			}
			// Converts a Quaternion to a Rotator
			static FRotator ToRotator(const FQuat& InputQuaternion)
			{
				return InputQuaternion.Rotator();
			}

			// Converts any numerical type to vector
			template <typename T>
			static FVector ToVector(const T& InputValue);
			// Converts a IntVector to a vector
			static FVector ToVector(const FIntVector& InputValue)
			{
				return FVector(InputValue.X, InputValue.Y, InputValue.Z);
			}
			// Converts a LinearColour to a vector
			static FVector ToVector(const FLinearColor& InputValue)
			{
				return FVector(InputValue);
			}
			// Converts a Vector4 to a vector
			static FVector ToVector(const FVector4& InputValue)
			{
				return FVector(InputValue);
			}
			// Converts a Vector2D to a vector
			static FVector ToVector(const FVector2D& InputValue, float Z = 0)
			{
				return FVector(InputValue, Z);
			}

			// converts any numeric value to boolean
			template<typename T>
			static bool ToBool(const T& Input);

			// converts any integral value to byte [uint8]
			template<typename T>
			static uint8 ToByte(const T& Input);

			// converts any arithmetical value to float
			template<typename T>
			static float ToFloat(const T& Input);

			// Converts any primitive type to a String
			// obviously it will not return a custom class or something
			// 
			// -----------------------------------
			// will treat the input as integer if not specified otherwise 
			// SYNTAX : e.g.: ToString<float>(200);
			// //	//	//	//	//
			// It will return the memory address of the type that cannot be converted
			template<typename T = int>
			static FString ToString(const T& InputWildcard);
			// Converts a Vector to a String
			static FString ToString(const FVector& InputVector)
			{
				return InputVector.ToString();
			}
			// Converts a Rotator to a String
			static FString ToString(const FRotator& InputRotator)
			{
				return InputRotator.ToString();
			}
			// Converts a Text to a String
			static FString ToString(const FText& InputText)
			{
				return InputText.ToString();
			}
			// Converts a Name to a String
			static FString ToString(const FName& InputName)
			{
				return InputName.ToString();
			}


			// Converts any primitve type to a Text
			// obviously it will not return a custom class or something
			//
			// -----------------------------------
			// will treat the input as integer if not specified otherwise 
			// SYNTAX : e.g.: ToText<float>(200);
			//	//	//	//	//	//
			// It will return the memory address of the type that cannot be converted
			template <typename T = int>
			static FText ToText(const T& InputWildcard)
			{
				return FText::AsCultureInvariant(ToString<T>(InputWildcard));
			}
			// Converts a Vector to a Text
			static FText ToText(const FVector& InputVector)
			{
				return FText::AsCultureInvariant(ToString(InputVector));
			}
			// Converts a Rotator to a Text
			static FText ToText(const FRotator& InputRotator)
			{
				return FText::AsCultureInvariant(ToString(InputRotator));
			}
			// Converts a String to a Text
			static FText ToText(const FString& InputString)
			{
				return FText::AsCultureInvariant(InputString);
			}
			// Converts a Name to a Text
			static FText ToText(const FName& InputName)
			{
				return FText::AsCultureInvariant(ToString(InputName));
			}
			

			// Converts any primitve type to a Name [if possible]
			// obviously it will not return a custom class or something
			//
			// -----------------------------------
			// will treat the input as integer if not specified otherwise 
			// SYNTAX : e.g.: ToName<float>(200);
			//	//	//	//	//	//
			// It will return the memory address of the type that cannot be converted
			template <typename T = int>
			static FName ToName(const T& InputWildcard)
			{
				return FName(*ToString<T>(InputWildcard));
			}
			// Converts a Vector to a Name
			static FName ToName(const FVector& InputVector)
			{
				return FName(*ToString(InputVector));
			}
			// Converts a Rotator to a Name
			static FName ToName(const FRotator& InputRotator)
			{
				return FName(*ToString(InputRotator));
			}
			// Converts a String to a Name
			static FName ToName(const FString& InputString)
			{
				return FName(*InputString);
			}
			// Converts a Text to a Name
			static FName ToName(const FText& InputText)
			{
				return ToName(ToString(InputText));
			}

			/*static FString GetDisplayName(const UObject* Object)
			{
				if (const AActor* Actor = Cast<const AActor>(Object))
				{
					return Actor->GetActorNameOrLabel();
				}
				else if (const UActorComponent* Component = Cast<const UActorComponent>(Object))
				{
					return Component->GetReadableName();
				}

				return Object ? Object->GetName() : FString();
			}*/

			// macro to add a space
			#define include_space (bIncludeSpaces ? " " : "")

			/*
			*	combines two strings together
			* 
			*	optionally you can included spaces between each String
			*/
			static FString Combine(FString A, FString B, bool bIncludeSpaces = true)
			{
				return A + include_space + B;
			}

			/*
			*	combines three strings together
			*
			*	optionally you can included spaces between each String
			*/
			static FString Combine(FString A, FString B, FString C, bool bIncludeSpaces = true)
			{
				return A + include_space + B + include_space + C;
			}

			/*
			*	combines four strings together
			*
			*	optionally you can included spaces between each String
			*/
			static FString Combine(FString A, FString B, FString C, FString D, bool bIncludeSpaces = true)
			{
				return A + include_space + B + include_space + C + include_space + D;
			}

			/*
			*	combines five strings together
			*
			*	optionally you can included spaces between each String
			*/
			static FString Combine(FString A, FString B, FString C, FString D, FString E, bool bIncludeSpaces = true)
			{
				return A + include_space + B + include_space + C + include_space + D + include_space + E;
			}
		};

		// Converts any numeric type to a Vector
		template<typename T>
		FORCEINLINE FVector TGenericConversions::ToVector(const T& InputValue)
		{
			// checks if the number inputted is a numeric value [int, float bool etc.]
			ensureArithmetic(T);

			// returns a new vector with the input parameter as coordinates [x,y,z]
			return FVector(InputValue);
		}

		// Converts any numeric type to a Boolean
		template<typename T>
		FORCEINLINE bool TGenericConversions::ToBool(const T& Input)
		{
			// checks if the number inputted is a numeric value [int, float etc.]
			ensureArithmetic(T);

			// returns true if the value is bigger than 0
			return Input > 0 ? true : false;
		}

		// Converts any integral type to a uint8 [Byte]
		template<typename T>
		FORCEINLINE uint8 TGenericConversions::ToByte(const T& Input)
		{
			// checks if the number inputted is an integer [int, int64, short etc]
			ensureAssert(T, std::is_integral, "Input Type mustn't be of type : 'Floating Point'");

			// converts it to uint8
			return (uint8)Input;
		}

		// Converts any arithmetical type to a Float
		template<typename T>
		FORCEINLINE float TGenericConversions::ToFloat(const T& Input)
		{
			// checks if the number inputted is a numeric value [int, float bool etc.]
			ensureArithmetic(T);

			// conversion 
			return (float)Input;
		}

		// Converts anything to a String
		template<typename T>
		FORCEINLINE FString TGenericConversions::ToString(const T& InputWildcard)
		{
			// safety assert for hopefully no crashes
			static_assert(std::is_arithmetic<T>::value || std::is_same<T, bool>::value, TEXT("Please select one of the primitive datatypes"));

			T CopiedInput = InputWildcard;
			FString FormattingType = TEXT("%i");

			// if the value is a boolean [specifically]
			if (std::is_same<T, bool>::value)
				return (bool)CopiedInput ? "true" : "false";

			// if the value is a number
			if (std::is_arithmetic<T>::value)
			{
				if (std::is_floating_point<T>::value)
					return FString::Printf(TEXT("%f"), (double)CopiedInput);
				else return FString::Printf(TEXT("%i"), (int)CopiedInput);
			}

			// return immediately if no datatype has been found
			return FString::Printf(TEXT("%i"), &InputWildcard);
		}
	}

	// Functionality for Streams [like in Java]
	namespace StreamsFunctionality
	{
		// sorting methods
		enum SortingType
		{
			normal,
			stable,
			heap
		};

		/*
			*
			*		- EXPERIMENTAL FUNCTIONALITY -
			*
			*	Allows for Streams... just like in Java ;)
			*
			*	Allows the user to chain functions together to create complex functionality with less code
			*	[This is generally slower than doing it the regular way.. but not necessarily noticeable in smaller Projects]
			*
			*	A type has to be selected for this to work.

		*/
		template<typename Datatype>
		class TStream
		{

			// returns itself -> for faster workflow [should only be accessed here if you use it for some reason 🤨]
			#define returnself	return *this
			// forced return type of this class for faster workflow [should only be accessed here if you use it for some reason 🤨]
			#define FORCERETURNSELF TStream<Datatype>
			// duplicate code wrapped in a macro [..]
			#define loop_and_calculate(arithmetic_operation) for (int i = 0; i < m_Data.Num(); i++) { m_Data[i] = m_Data[i] arithmetic_operation by; }

			//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//

			// The Data that is being stored
			TArray<Datatype> m_Data;

		public:

			// Default Constructor with no input [the only option for construction]
			TStream() { }

				// All operations

			// allows the user to set an array to the Stream Class Instance
			FORCERETURNSELF operator=(const TArray<Datatype>& Array)
			{
				SetArrayToThis(Array);
				returnself;
			}

				// Public Functions that can be chained since all of them return the class itself [..]

			// Adds an Element to the List
			FORCERETURNSELF add(const Datatype& value)
			{
				m_Data.Add(value);
				returnself;
			}
			// Adds Elements to the List
			FORCERETURNSELF add(TArray<Datatype> values)
			{
				for (const auto& valuesingle : values)
				{
					add(valuesingle);
				}

				returnself;
			}

			// Removes an Element from the List
			FORCERETURNSELF remove(const Datatype& value)
			{
				m_Data.Remove(value);
				returnself;
			}
			// Removes Elements from the List
			FORCERETURNSELF remove(TArray<Datatype> values)
			{
				for (const auto& valuesingle : values)
				{
					remove(valuesingle);
				}

				returnself;
			}

			// some sorting algorithms
			FORCERETURNSELF sort(TEnumAsByte<SortingType> sortingtype = stable)
			{
				switch (sortingtype)
				{
				case normal:
					m_Data.Sort();
					break;
				case stable:
					m_Data.StableSort();
					break;
				case heap:
					m_Data.HeapSort();
					break;
				}

				returnself;
			}

			//// loops through the list
			//TStream<Datatype> foreach(std::function<void(Datatype)> func)
			//{
			//	for (const auto& datasingle : m_Data)
			//	{
			//		// does the inputted function for each of the elements in the list
			//		func(datasingle);
			//	}

			//	returnself;
			//}

			// returns the highest value from the list
			FORCERETURNSELF max(Datatype& returnvalue)
			{
				if (std::is_arithmetic<Datatype>::value)
				{
					Datatype value = 0;
					for (const auto& datasingle : m_Data)
					{
						if (datasingle > value)
							value = datasingle;
					}

					returnvalue = value;
				}

				returnself;
			}

			// returns the lowest value from the list
			FORCERETURNSELF min(Datatype& returnvalue)
			{
				if (std::is_arithmetic<Datatype>::value)
				{
					Datatype value = 0;
					for (const auto& datasingle : m_Data)
					{
						if (datasingle < value)
							value = datasingle;
					}

					returnvalue = value;
				}

				returnself;
			}

			// filters the string using a condition
			FORCERETURNSELF filter(std::function<bool(Datatype)> func)
			{
				TArray<Datatype> tmp_Data;
				for (const auto& datasingle : m_Data)
				{
					// if this function returns true then the value should stay in the array
					// remove if not
					if (func(datasingle))
						tmp_Data.Add(datasingle);
				}

				m_Data = tmp_Data;
				returnself;
			}

			// applies a function to each element in the list
			FORCERETURNSELF map(std::function<Datatype(Datatype)> func)
			{
				for (int i = 0; i < m_Data.Num(); i++)
				{
					// replaces the value from the list with the new value which got "calculated" in the custom function
					m_Data[i] = func(m_Data[i]);
				}

				returnself;
			}

			// applies a function to each element in the list
			// allows for change in its datatype
			template <typename To>
			TStream<To> map_to(std::function<To(Datatype)> func)
			{
				TStream<To> NewStreamList;
				for (int i = 0; i < m_Data.Num(); i++)
				{
					// replaces the value from the list with the new value which got "calculated" in the custom function
					NewStreamList.add(func(m_Data[i]));
				}

				return NewStreamList;
			}

				// Arithmetic Operations on stream

			// multiplication for each element in the list
			FORCERETURNSELF multiply(Datatype by)
			{
				loop_and_calculate(*)
					returnself;
			}
			// division for each element in the list
			FORCERETURNSELF divide(Datatype by)
			{
				loop_and_calculate(/)
					returnself;
			}
			// addition for each element in the list
			FORCERETURNSELF addition(Datatype by)
			{
				loop_and_calculate(+)
					returnself;
			}
			// subtraction for each element in the list
			FORCERETURNSELF subtraction(Datatype by)
			{
				loop_and_calculate(-)
					returnself;
			}

			// find element based on conditions
			Datatype find(std::function<bool(Datatype)> condition)
			{
				for (const auto& datasingle : m_Data)
				{
					// if this function returns true then the value should be returned
					// continue if not
					if (condition(datasingle))
						return datasingle;
				}

				return NULL;
			}

			//	//	//	//	//	//

			// returns the amount of elements
			int count()
			{
				return m_Data.Num();
			}

			// sums the list
			Datatype sum()
			{
				if (!std::is_arithmetic<Datatype>::value)
					return 0;

				Datatype summed_data;
				for (const auto& datasingle : m_Data)
				{
					summed_data += datasingle;
				}

				return summed_data;
			}

			// returns the stream as an array and closes the stream
			TArray<Datatype> close()
			{
				return;
			}

		private:

			// Sets the Array Elements to this class
			void SetArrayToThis(const TArray<Datatype>& Array)
			{
				m_Data = Array;
			}
		};

	}

	//namespace AdditionalFunctionalities
	//{
	//	struct ADVDATATYPESFORCPP_API TFunc
	//	{
	//		static float time_value;


	//		// will generate a smooth curve interpolating from 0 to 1 to -1 to 1 again
	//		static float TimedSine(UObject* WorldContext)
	//		{
	//			return FMath::Sin(get_time_value(WorldContext));
	//		}

	//		// will generate a smooth curve interpolating from 1 to -1
	//		static float TimedCoSine(UObject* WorldContext)
	//		{
	//			return FMath::Cos(get_time_value(WorldContext));
	//		}

	//	private:

	//		// a global function which will have the same value regardless of where it gets called
	//		static float get_time_value(UObject* WorldContext)
	//		{
	//			ensureWorld(WorldContext) 0.0;
	//			const auto DeltaSeconds = UGameplayStatics::GetWorldDeltaSeconds(GetWorld);

	//			return time_value = time_value + DeltaSeconds;
	//		}
	//	};
	//}
}

// easier access to the respective class
using FInt = AdvDatatypesAndConversions::Arithmetics::IntegralType::TInt;				// Integer	Datatype
using FBool = AdvDatatypesAndConversions::Arithmetics::BooleanType::TBool;				// Boolean	Datatype
using FFloat = AdvDatatypesAndConversions::Arithmetics::FloatationType::TFloat;			// Float	Datatype
using FDouble = AdvDatatypesAndConversions::Arithmetics::FloatationType::TDouble;		// Double	Datatype
//	//	//	//	//	//	//	//
using FConv = AdvDatatypesAndConversions::ExtraConversions::TGenericConversions;		// Conversion Functions
//	//	//
// using FFunc = AdvDatatypesAndConversions::AdditionalFunctionalities::TFunc;				// Additional Functions usable in C++

//	//	//	Other Functionality	//	//	//

// !! EXPERIMENTAL !! : Streams Functionality
template<typename T>
using FStream = AdvDatatypesAndConversions::StreamsFunctionality::TStream<T>;			// !! EXPERIMENTAL !! : Streams Functionality
//	//	//	//
using ESortingType = AdvDatatypesAndConversions::StreamsFunctionality::SortingType;		// Enum for selecting the proper sorting type