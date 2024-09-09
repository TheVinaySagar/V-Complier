#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
using namespace std;

enum TokenKind
{
	EndoffileToken,
	Numbertoken,
	whitespaceToken,
	PlusToken,
	minusToken,
	StarToken,
	divisionToken,
	badToken,
	openparenthasisToken,
	closeparenthasisToken,
	NumberExpression,
	BinaryExpression
};

inline const char* ToString(TokenKind color) {
	if (color == 0)
	{
		return "EndoffileToken";
	}
	if (color == 1)
	{
		return "Numbertoken";
	}
	if (color == 2)
	{
		return "whitespaceToken";
	}
	if (color == 3)
	{
		return "PlusToken";
	}
	if (color == 4)
	{
		return "minusToken";
	}
	if (color == 5)
	{
		return "MultiplyToken";
	}
	if (color == 6)
	{
		return "divisionToken";
	}
	if (color == 7)
	{
		return "BadToken";
	}
	if (color == 8)
	{
		return "openparenthasisToken";
	}
	if (color == 9)
	{
		return "closeparenthasisToken";
	}
}

class SyntexNode
{
public:
	TokenKind kind;
	virtual TokenKind kindover() { return kind; };

	virtual vector<SyntexNode> GetChildren();
};

class SyntexToken : SyntexNode
{
public:
	TokenKind Kind;
	int Position;
	string Text;
	int Value;

	//SyntexToken() {} 
	SyntexToken() = default;
	SyntexToken(TokenKind kind, int position, string text, int value)
	{
		Kind = kind;
		Position = position;
		Text = text;
		Value = value;
	}

	vector<SyntexNode> GetChildren() override
	{
		return vector<SyntexNode>();
	}
};

// Lexar - it simply identify the type of token by traversing the whole string.
class lexar
{
public:
	string string_text;
	int _position = 0;
	lexar(string text)
	{
		string_text = text;
	}

	// tells the current character at which the "_position" is at.
	char current()
	{
		return string_text[_position];
	}

	// It increment the "_position" variable by 1 - which then point to next char.
	void Next()
	{
		_position++;
	}


	// This is the main game which detect the token in string and form syntexToken object to each.
	SyntexToken NextToken()
	{
		// <number> 
		// + - * / ( ) 
		// whitespaces

		//End of the file
		if (_position >= string_text.length())
		{
			TokenKind kk = EndoffileToken;
			SyntexToken object(kk, _position, "", NULL);
			return object;
		}

		//Number
		if (isdigit(current()))
		{
			int start = _position;
			while (isdigit(current()))
				Next();

			int lenght = _position - start;
			string text = string_text.substr(start, lenght);
			int value = stoi(text);
			SyntexToken object(TokenKind::Numbertoken, start, text, value);
			return object;
		}

		// is a space 
		if (isspace(current()))
		{
			int start = _position;

			while (isspace(current()))
				Next();

			int length = _position - start;
			string text = string_text.substr(start, length);
			SyntexToken object(TokenKind::whitespaceToken, _position, text, -1);
			return object;
		}

		// '+' token
		if (current() == '+')
		{
			TokenKind kk = PlusToken;
			SyntexToken object(kk, _position++, "+", -1);
			return object;
		}

		// '-' token 
		else if (current() == '-')
		{
			TokenKind kk = minusToken;
			SyntexToken object(kk, _position++, "-", -1);
			return object;
		}

		// '*' token 
		else if (current() == '*')
		{
			TokenKind kk = StarToken;
			SyntexToken object(kk, _position++, "*", -1);
			return object;
		}

		// '/' token
		else if (current() == '/')
		{
			TokenKind kk = divisionToken;
			SyntexToken object(kk, _position++, "/", -1);
			return object;
		}

		// '(' token 
		else if (current() == '(')
		{
			TokenKind kk = openparenthasisToken;
			SyntexToken object(kk, _position++, "(", -1);
			return object;
		}

		// ')' token
		else if (current() == ')')
		{
			TokenKind kk = closeparenthasisToken;
			SyntexToken object(kk, _position++, ")", -1);
			return object;
		}

		// BadToken
		else
		{
			TokenKind kk = badToken;
			SyntexToken object(kk, _position++, "", -1);
			return object;
		}
	}
};

class ExpressionSyntex : SyntexNode
{

};

class NumberExpressionSyntex : public ExpressionSyntex
{
public:
	SyntexToken NumberToken;

	NumberExpressionSyntex(SyntexToken numberToken)
	{
		NumberToken = numberToken;
	}

	TokenKind kindover() override { return TokenKind::NumberExpression; }

	vector<SyntexNode> GetChildren() override
	{
		return NumberToken;
	}
};

class BinaryExpressionSyntex : public ExpressionSyntex
{
public:
	SyntexToken BinaryToken;
	ExpressionSyntex Left;
	ExpressionSyntex Right;
	BinaryExpressionSyntex(ExpressionSyntex left, SyntexToken binaryToken, ExpressionSyntex right)
	{
		Left = left;
		BinaryToken = binaryToken;
		Right = right;
	}

	TokenKind kindover() override { return TokenKind::BinaryExpression; }
};

class Parsor
{
public:
	vector<SyntexToken> tokens;
	int _position;
	Parsor(string text)
	{
		lexar Lexar(text);
		SyntexToken token;
		do
		{
			token = Lexar.NextToken();
			if (token.Kind != 2 && token.Kind != 7)
			{
				tokens.push_back(token);
			}
		} while (token.Kind != 0);
	}

private:
	SyntexToken giveMeThatToken(int offset)
	{
		int index = _position + offset;
		if (offset >= (tokens.size() - 1))
			return tokens[tokens.size() - 1];
		return tokens[index];
	}
	SyntexToken currentToken()
	{
		return giveMeThatToken(0);
	}

	SyntexToken	NextToken()
	{
		SyntexToken current = currentToken();
		_position++;
		return current;
	}

	SyntexToken Match(TokenKind kind)
	{
		auto token = currentToken();
		if (token.Kind == kind)
		{
			return NextToken();
		}

		SyntexToken object = SyntexToken(kind, token.Position, " ", -1);
		return object;
	}

public:
	ExpressionSyntex Parse()
	{
		auto left = ParsePrimaryExpression();

		auto newToken = currentToken();

		while (newToken.Kind == TokenKind::PlusToken || newToken.Kind == TokenKind::minusToken)
		{
			auto NextT = NextToken();
			auto right = ParsePrimaryExpression();
			left = BinaryExpressionSyntex(left, NextT, right);
		}
	}

private:
	ExpressionSyntex ParsePrimaryExpression()
	{
		auto numberToken = Match(TokenKind::Numbertoken);
		NumberExpressionSyntex object = NumberExpressionSyntex(numberToken);
		return object;
	}
};

void main()
{
	while (true)
	{
		cout << "> ";
		string line;
		getline(cin, line);


		if (line.empty())
			return;

		lexar lexa(line);
		while (true)
		{
			SyntexToken token = lexa.NextToken();
			TokenKind kin = token.Kind;
			if (kin == 0)
				break;
			//cout<< endl;
			if (token.Value != -1)
				cout << ToString(kin) << ":" << "'" << token.Text << "'" << " " << token.Value << endl;
			else
				cout << ToString(kin) << ":" << "'" << token.Text << "'" << endl;
		}

	}
	return;
}
