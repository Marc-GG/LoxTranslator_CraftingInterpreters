#include "variant"
#include "../Lox/Token.h"
class Visitor;
class Expr {
public: 
  virtual void accept(Visitor* visitor) const = 0;

};
  struct Binary : public Expr {
    const Expr* left;

    const Token* oper;

    const Expr* right;

    Binary(Expr* left, Token* oper, Expr* right) {
      this->left = left;
      this->oper = oper;
      this->right = right;
    }

    virtual void accept(Visitor* visitor) const override {
      return visitor->visitBinaryExpr(this);
    }
  };

  struct Grouping : public Expr {
    const Expr* expression;

    Grouping(Expr* expression) {
      this->expression = expression;
    }

    virtual void accept(Visitor* visitor) const override {
      return visitor->visitGroupingExpr(this);
    }
  };

  struct Literal : public Expr {
    const LoxValue* value;

    Literal(LoxValue* value) {
      this->value = value;
    }

    virtual void accept(Visitor* visitor) const override {
      return visitor->visitLiteralExpr(this);
    }
  };

  struct Unary : public Expr {
    const Token* oper;

    const Expr* right;

    Unary(Token* oper, Expr* right) {
      this->oper = oper;
      this->right = right;
    }

    virtual void accept(Visitor* visitor) const override {
      return visitor->visitUnaryExpr(this);
    }
  };

  class Visitor {
    public: 
    virtual void visitBinaryExpr(const Binary* Expr) = 0;
    virtual void visitGroupingExpr(const Grouping* Expr) = 0;
    virtual void visitLiteralExpr(const Literal* Expr) = 0;
    virtual void visitUnaryExpr(const Unary* Expr) = 0;
  };
