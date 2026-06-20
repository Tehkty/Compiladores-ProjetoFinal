#ifndef AST_HPP
#define AST_HPP

#include <memory>
#include <string>
#include <vector>

class CodeGenContext;

struct Expr {
    virtual ~Expr() = default;
    virtual std::string codegen(CodeGenContext& ctx) = 0;
};

struct NumberExpr : Expr {
    int value;
    explicit NumberExpr(int v) : value(v) {}
    std::string codegen(CodeGenContext& ctx) override;
};

struct StringExpr : Expr {
    std::string value;
    explicit StringExpr(std::string v) : value(std::move(v)) {}
    std::string codegen(CodeGenContext& ctx) override;
};

struct VariableExpr : Expr {
    std::string name;
    explicit VariableExpr(std::string n) : name(std::move(n)) {}
    std::string codegen(CodeGenContext& ctx) override;
};

struct BinaryExpr : Expr {
    std::string op;
    std::unique_ptr<Expr> lhs;
    std::unique_ptr<Expr> rhs;
    BinaryExpr(std::string o, Expr* l, Expr* r) : op(std::move(o)), lhs(l), rhs(r) {}
    std::string codegen(CodeGenContext& ctx) override;
};

struct ArgIntExpr : Expr {
    std::unique_ptr<Expr> index;
    explicit ArgIntExpr(Expr* i) : index(i) {}
    std::string codegen(CodeGenContext& ctx) override;
};

struct Stmt {
    virtual ~Stmt() = default;
    virtual void codegen(CodeGenContext& ctx) = 0;
};

struct BlockStmt : Stmt {
    std::vector<std::unique_ptr<Stmt>> statements;
    explicit BlockStmt(std::vector<Stmt*>* stmts);
    void codegen(CodeGenContext& ctx) override;
};

struct AssignStmt : Stmt {
    std::string name;
    std::unique_ptr<Expr> expr;
    AssignStmt(std::string n, Expr* e) : name(std::move(n)), expr(e) {}
    void codegen(CodeGenContext& ctx) override;
};

struct WriteStmt : Stmt {
    bool newline;
    std::vector<std::unique_ptr<Expr>> args;
    WriteStmt(bool nl, std::vector<Expr*>* a);
    void codegen(CodeGenContext& ctx) override;
};

struct IfStmt : Stmt {
    std::unique_ptr<Expr> cond;
    std::vector<std::unique_ptr<Stmt>> thenStmts;
    std::vector<std::unique_ptr<Stmt>> elseStmts;
    IfStmt(Expr* c, std::vector<Stmt*>* t, std::vector<Stmt*>* e);
    void codegen(CodeGenContext& ctx) override;
};

struct WhileStmt : Stmt {
    std::unique_ptr<Expr> cond;
    std::vector<std::unique_ptr<Stmt>> body;
    WhileStmt(Expr* c, std::vector<Stmt*>* b);
    void codegen(CodeGenContext& ctx) override;
};

struct ForStmt : Stmt {
    std::string var;
    std::unique_ptr<Expr> start;
    std::unique_ptr<Expr> end;
    std::vector<std::unique_ptr<Stmt>> body;
    ForStmt(std::string v, Expr* s, Expr* e, std::vector<Stmt*>* b);
    void codegen(CodeGenContext& ctx) override;
};

struct Program {
    std::string name;
    std::vector<std::string> variables;
    std::vector<std::unique_ptr<Stmt>> statements;
    Program(std::string n, std::vector<std::string>* vars, std::vector<Stmt*>* stmts);
    void codegen(CodeGenContext& ctx);
};

#endif
