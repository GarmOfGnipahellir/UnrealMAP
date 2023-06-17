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
	In1 = Ignore(In1);
	auto [In2, y] = Float(In1);
	In2 = Ignore(In2);
	auto [In3, z] = Float(In2);

	return TTuple<FString, FVector>(In3, FVector(x, y, z));
}

TTuple<FString, FString> FMAPParser::Comment(const FString& Input)
{
	try
	{
		auto In1 = Tag("//", Input).Key;
		FString Out;
		for (const auto c : In1)
		{
			if (c == '\n') break;

			Out += c;
		}
		return TTuple<FString, FString>(In1.Right(In1.Len() - Out.Len()), Out);
	}
	catch (const std::exception&)
	{
		return TTuple<FString, FString>(Input, "");
	}
}

TTuple<FString, TArray<FString>> FMAPParser::Comments(const FString& Input)
{
	FString In1 = Input;
	TArray<FString> Out;
	while (true)
	{
		try
		{
			Tag("//", In1);
		}
		catch (const std::exception&)
		{
			break;
		}

		auto [In2, Text] = Comment(In1);
		In2 = Whitespace(In2).Key;
		Out.Add(Text);
		In1 = In2;
	}
	return TTuple<FString, TArray<FString>>(In1, Out);
}

FString FMAPParser::Ignore(const FString& Input)
{
	auto In1 = Whitespace(Input).Key;
	In1 = Comments(In1).Key;
	In1 = Whitespace(In1).Key;
	return In1;
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
	auto In1 = Comment(Input).Key;
	In1 = Ignore(In1);
	In1 = Tag("{", In1).Key;
	In1 = Ignore(In1);
	TArray<FMAPFace> Faces;
	while (true)
	{
		try
		{
			auto [In2, Face] = MAPFace(In1);
			In2 = Ignore(In2);
			Faces.Add(Face);
			In1 = In2;
		}
		catch (const std::exception&)
		{
			break;
		}
	}
	In1 = Ignore(In1);
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
			In2 = Ignore(In2);
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

TTuple<FString, FMAPEntity> FMAPParser::MAPEntity(const FString& Input)
{
	auto In1 = Comment(Input).Key;
	In1 = Ignore(In1);
	In1 = Tag("{", In1).Key;
	In1 = Ignore(In1);
	auto [In2, Properties] = MAPProperties(In1);
	In2 = Ignore(In2);
	TArray<FMAPBrush> Brushes;
	while (true)
	{
		try
		{
			auto [In3, Brush] = MAPBrush(In2);
			In3 = Ignore(In3);
			Brushes.Add(Brush);
			In2 = In3;
		}
		catch (const std::exception&)
		{
			break;
		}
	}
	In2 = Ignore(In2);
	In2 = Tag("}", In2).Key;

	return TTuple<FString, FMAPEntity>(In2, FMAPEntity{Properties, Brushes});
}

TTuple<FString, FMAPMap> FMAPParser::MAPMap(const FString& Input)
{
	auto In1 = Ignore(Input);
	TArray<FMAPEntity> Entities;
	while (true)
	{
		try
		{
			auto [In2, Entity] = MAPEntity(In1);
			In2 = Ignore(In2);
			Entities.Add(Entity);
			In1 = In2;
		}
		catch (const std::exception&)
		{
			break;
		}
	}

	return TTuple<FString, FMAPMap>(In1, FMAPMap{Entities});
}
