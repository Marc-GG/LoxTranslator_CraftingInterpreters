#pragma once
#include "Token.h"
#include "Expr.h"
#include <memory>
#include <vector>
struct Block;
struct Expression;
struct If;
struct Print;
struct Var;
struct While;
  class StmtVisitor {
    public: 
    virtual void visitBlockStmt(const Block* Stmt) = 0;
    virtual void visitExpressionStmt(const Expression* Stmt) = 0;
    virtual void visitIfStmt(const If* Stmt) = 0;
    virtual void visitPrintStmt(const Print* Stmt) = 0;
    virtual void visitVarStmt(const Var* Stmt) = 0;
    virtual void visitWhileStmt(const While* Stmt) = 0;
  };
class Stmt {
public: 
  virtual void accept(StmtVisitor* visitor) const = 0;

};
  struct Block : public Stmt {
    const vector<Stmt*> statements; // const?

    Block(vector<Stmt*> statements) : 
       statements{statements} {
     }

    virtual void accept(StmtVisitor* visitor) const override {
      return visitor->visitBlockStmt(this);
    }
  };

  struct Expression : public Stmt {
    const Expr* expression;

    Expression(Expr* expression) : 
       expression{expression} {
     }

    virtual void accept(StmtVisitor* visitor) const override {
      return visitor->visitExpressionStmt(this);
    }
  };

  struct If : public Stmt {
    const Expr* condition;

    const Stmt* thenBranch;

    const Stmt* elseBranch;

    If(Expr* condition, Stmt* thenBranch, Stmt* elseBranch) : 
       condition{condition},       thenBranch{thenBranch},       elseBranch{elseBranch} {
     }

    virtual void accept(StmtVisitor* visitor) const override {
      return visitor->visitIfStmt(this);
    }
  };

  struct Print : public Stmt {
    const Expr* expression;

    Print(Expr* expression) : 
       expression{expression} {
     }

    virtual void accept(StmtVisitor* visitor) const override {
      return visitor->visitPrintStmt(this);
    }
  };

  struct Var : public Stmt {
    const Token name;

    const Expr* initializer;

    Var(Token name, Expr* initializer) : 
       name{name},       initializer{initializer} {
     }

    virtual void accept(StmtVisitor* visitor) const override {
      return visitor->visitVarStmt(this);
    }
  };

  struct While : public Stmt {
    const Expr* condition;

    const Stmt* body;

    While(Expr* condition, Stmt* body) : 
       condition{condition},       body{body} {
     }

    virtual void accept(StmtVisitor* visitor) const override {
      return visitor->visitWhileStmt(this);
    }
  };

