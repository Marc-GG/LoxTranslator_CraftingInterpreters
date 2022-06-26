#pragma once
#include "Token.h"
struct Assign;
struct Binary;
struct Grouping;
struct Literal;
struct Logical;
struct Unary;
struct Variable;
  class ExprVisitor {
    public: 
    virtual void visitAssignExpr(const Assign* Expr) = 0;
    virtual void visitBinaryExpr(const Binary* Expr) = 0;
    virtual void visitGroupingExpr(const Grouping* Expr) = 0;
    virtual void visitLiteralExpr(const Literal* Expr) = 0;
    virtual void visitLogicalExpr(const Logical* Expr) = 0;
    virtual void visitUnaryExpr(const Unary* Expr) = 0;
    virtual void visitVariableExpr(const Variable* Expr) = 0;
  };
class Expr {
public: 
  virtual void accept(ExprVisitor* visitor) const = 0;

};
  struct Assign : public Expr {
    const Token name;

    const Expr* value;

    Assign(Token name, Expr* value) : 
       name{name},       value{value} {
     }

    virtual void accept(ExprVisitor* visitor) const override {
      return visitor->visitAssignExpr(this);
    }
  };

  struct Binary : public Expr {
    const Expr* left;

    const Token oper;

    const Expr* right;

    Binary(Expr* left, Token oper, Expr* right) : 
       left{left},       oper{oper},       right{right} {
     }

    virtual void accept(ExprVisitor* visitor) const override {
      return visitor->visitBinaryExpr(this);
    }
  };

  struct Grouping : public Expr {
    const Expr* expression;

    Grouping(Expr* expression) : 
       expression{expression} {
     }

    virtual void accept(ExprVisitor* visitor) const override {
      return visitor->visitGroupingExpr(this);
    }
  };

  struct Literal : public Expr {
    const LoxValue value;

    Literal(LoxValue value) : 
       value{value} {
     }

    virtual void accept(ExprVisitor* visitor) const override {
      return visitor->visitLiteralExpr(this);
    }
  };

  struct Logical : public Expr {
    const Expr* left;

    const Token oper;

    const Expr* right;

    Logical(Expr* left, Token oper, Expr* right) : 
       left{left},       oper{oper},       right{right} {
     }

    virtual void accept(ExprVisitor* visitor) const override {
      return visitor->visitLogicalExpr(this);
    }
  };

  struct Unary : public Expr {
    const Token oper;

    const Expr* right;

    Unary(Token oper, Expr* right) : 
       oper{oper},       right{right} {
     }

    virtual void accept(ExprVisitor* visitor) const override {
      return visitor->visitUnaryExpr(this);
    }
  };

  struct Variable : public Expr {
    const Token name;

    Variable(Token name) : 
       name{name} {
     }

    virtual void accept(ExprVisitor* visitor) const override {
      return visitor->visitVariableExpr(this);
    }
  };

