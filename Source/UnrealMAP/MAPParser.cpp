// Fill out your copyright notice in the Description page of Project Settings.


#include "MAPParser.h"
#include "MAPModel.h"

#include <cwctype>

TTuple<FString, FString> FMAPParser::Tag(const FString& Tag, const FString& Input)
{
	if (!Input.StartsWith(Tag))
	{
		throw std::exception("Unmatched tag");
	}

	return TTuple<FString, FString>(
		Input.Right(Input.Len() - Tag.Len()),
		Input.Left(Tag.Len())
	);
}

TTuple<FString, FString> FMAPParser::NotChar(const wchar_t& Char, const FString& Input, const bool bOneOrMore)
{
	FString Out;
	for (const auto c : Input)
	{
		if (c == Char) break;

		Out += c;
	}

	if (bOneOrMore && Out.IsEmpty())
	{
		throw std::exception("Unmatched not char");
	}

	return TTuple<FString, FString>(
		Input.Right(Input.Len() - Out.Len()),
		Out
	);
}

TTuple<FString, float> FMAPParser::Float(const FString& Input)
{
	FString Text;
	for (const auto c : Input)
	{
		if (c != '-' && c != '.' && !std::iswdigit(c)) break;

		Text += c;
	}

	if (!Text.IsNumeric())
	{
		throw std::exception("Unmatched float");
	}

	return TTuple<FString, float>(
		Input.Right(Input.Len() - Text.Len()),
		FCString::Atof(*Text)
	);
}

TTuple<FString, FString> FMAPParser::Whitespace(const FString& Input, const bool bOneOrMore)
{
	FString Out;
	for (const auto c : Input)
	{
		if (!std::iswspace(c)) break;

		Out += c;
	}

	if (bOneOrMore && Out.IsEmpty())
	{
		throw std::exception("Unmatched whitespace");
	}

	return TTuple<FString, FString>(
		Input.Right(Input.Len() - Out.Len()),
		Out
	);
}

TTuple<FString, FString> FMAPParser::NotWhitespace(const FString& Input, bool bOneOrMore)
{
	FString Out;
	for (const auto c : Input)
	{
		if (std::iswspace(c)) break;

		Out += c;
	}

	if (bOneOrMore && Out.IsEmpty())
	{
		throw std::exception("Unmatched not whitespace");
	}

	return TTuple<FString, FString>(
		Input.Right(Input.Len() - Out.Len()),
		Out
	);
}

TTuple<FString, FVector> FMAPParser::Vector(const FString& Input)
{
	auto [In1, x] = Float(Input);
	In1 = Whitespace(In1, true).Key;
	auto [In2, y] = Float(In1);
	In2 = Whitespace(In2, true).Key;
	auto [In3, z] = Float(In2);

	return TTuple<FString, FVector>(In3, FVector(x, y, z));
}

TTuple<FString, FMAPPlane> FMAPParser::MAPPlane(const FString& Input)
{
	auto In1 = Tag("(", Input).Key;
	In1 = Whitespace(In1).Key;
	auto [In2, P0] = Vector(In1);
	In2 = Whitespace(In2).Key;
	In2 = Tag(")", In2).Key;
	In2 = Whitespace(In2, true).Key;
	In2 = Tag("(", In2).Key;
	In2 = Whitespace(In2).Key;
	auto [In3, P1] = Vector(In2);
	In3 = Whitespace(In3).Key;
	In3 = Tag(")", In3).Key;
	In3 = Whitespace(In3, true).Key;
	In3 = Tag("(", In3).Key;
	In3 = Whitespace(In3).Key;
	auto [In4, P2] = Vector(In3);
	In4 = Whitespace(In4).Key;
	In4 = Tag(")", In4).Key;

	return TTuple<FString, FMAPPlane>(In4, FMAPPlane{P0, P1, P2});
}


TTuple<FString, FMAPFace> FMAPParser::MAPFace(const FString& Input)
{
	auto [In1, Plane] = MAPPlane(Input);
	In1 = Whitespace(In1, true).Key;
	auto [In2, Texture] = NotWhitespace(In1);
	In2 = Whitespace(In2, true).Key;
	In2 = Tag("[", In2).Key;
	In2 = Whitespace(In2).Key;
	auto [In3, U] = Vector(In2);
	In3 = Whitespace(In3, true).Key;
	auto [In4, OffsetX] = Float(In3);
	In4 = Whitespace(In4).Key;
	In4 = Tag("]", In4).Key;
	In4 = Whitespace(In4, true).Key;
	In4 = Tag("[", In4).Key;
	In4 = Whitespace(In4).Key;
	auto [In5, V] = Vector(In4);
	In5 = Whitespace(In5, true).Key;
	auto [In6, OffsetY] = Float(In5);
	In6 = Whitespace(In6).Key;
	In6 = Tag("]", In6).Key;
	In6 = Whitespace(In6, true).Key;
	auto [In7, Rotation] = Float(In6);
	In7 = Whitespace(In7, true).Key;
	auto [In8, ScaleX] = Float(In7);
	In8 = Whitespace(In8, true).Key;
	auto [In9, ScaleY] = Float(In8);

	return TTuple<FString, FMAPFace>(
		In9,
		FMAPFace{Plane, Texture, U, V, FVector2d(OffsetX, OffsetY), Rotation, FVector2d(ScaleX, ScaleY)}
	);
}

TTuple<FString, FMAPBrush> FMAPParser::MAPBrush(const FString& Input)
{
	auto In1 = Tag("{", Input).Key;
	In1 = Whitespace(In1).Key;
	TArray<FMAPFace> Faces;
	while (true)
	{
		try
		{
			auto [In2, Face] = MAPFace(In1);
			In2 = Whitespace(In2).Key;
			Faces.Add(Face);
			In1 = In2;
		}
		catch (const std::exception&)
		{
			break;
		}
	}
	In1 = Whitespace(In1).Key;
	In1 = Tag("}", In1).Key;

	return TTuple<FString, FMAPBrush>(In1, FMAPBrush{Faces});
}

TTuple<FString, TTuple<FString, FString>> FMAPParser::MAPProperty(const FString& Input)
{
	auto In1 = Tag("\"", Input).Key;
	auto [In2, Key] = NotChar('"', In1);
	In2 = Tag("\"", In2).Key;
	In2 = Whitespace(In2, true).Key;
	In2 = Tag("\"", In2).Key;
	auto [In3, Value] = NotChar('"', In2);
	In3 = Tag("\"", In3).Key;

	return TTuple<FString, TTuple<FString, FString>>(In3, TTuple<FString, FString>(Key, Value));
}

TTuple<FString, TMap<FString, FString>> FMAPParser::MAPProperties(const FString& Input)
{
	FString In1 = Input;
	TMap<FString, FString> Properties;
	while (true)
	{
		try
		{
			auto [In2, Property] = MAPProperty(In1);
			In2 = Whitespace(In2).Key;
			Properties.Add(Property);
			In1 = In2;
		}
		catch (const std::exception&)
		{
			break;
		}
	}

	return TTuple<FString, TMap<FString, FString>>(In1, Properties);
}
