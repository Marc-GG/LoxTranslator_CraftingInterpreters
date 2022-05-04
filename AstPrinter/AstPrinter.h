#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <map>
#include <variant>
#include <string>
#include "Expr.h"

using std::string;

class AstPrinter : public Visitor {
	string result;
	string parenthesize(string name, vector<const Expr*> exprs);
public:

	string print(Expr* expr);

	AstPrinter() : result{ "" } {}

	virtual void visitBinaryExpr(const Binary* expr) override;

	virtual void visitGroupingExpr(const Grouping* expr) override;

	virtual void visitLiteralExpr(const Literal* expr) override;

	virtual void visitUnaryExpr(const Unary* expr) override;

};