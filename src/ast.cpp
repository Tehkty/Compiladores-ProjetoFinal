#include "ast.hpp"
#include "codegen.hpp"
#include <iostream>

static void moveStmtVector(std::vector<std::unique_ptr<Stmt>>& dst, std::vector<Stmt*>* src) {
    if (!src) return;
    for (auto* s : *src) dst.emplace_back(s);
    delete src;
}

BlockStmt::BlockStmt(std::vector<Stmt*>* stmts) { moveStmtVector(statements, stmts); }
void BlockStmt::codegen(CodeGenContext& ctx) { for (auto& s : statements) s->codegen(ctx); }

WriteStmt::WriteStmt(bool nl, std::vector<Expr*>* a) : newline(nl) {
    if (a) { for (auto* e : *a) args.emplace_back(e); delete a; }
}
IfStmt::IfStmt(Expr* c, std::vector<Stmt*>* t, std::vector<Stmt*>* e) : cond(c) { moveStmtVector(thenStmts, t); moveStmtVector(elseStmts, e); }
WhileStmt::WhileStmt(Expr* c, std::vector<Stmt*>* b) : cond(c) { moveStmtVector(body, b); }
ForStmt::ForStmt(std::string v, Expr* s, Expr* e, std::vector<Stmt*>* b) : var(std::move(v)), start(s), end(e) { moveStmtVector(body, b); }
Program::Program(std::string n, std::vector<std::string>* vars, std::vector<Stmt*>* stmts) : name(std::move(n)) {
    if (vars) { variables = *vars; delete vars; }
    moveStmtVector(statements, stmts);
}

std::string NumberExpr::codegen(CodeGenContext&) { return std::to_string(value); }

std::string StringExpr::codegen(CodeGenContext& ctx) { return ctx.createStringConstant(value); }

std::string VariableExpr::codegen(CodeGenContext& ctx) {
    if (!ctx.symbols.exists(name)) std::cerr << "Erro semantico: variavel nao declarada: " << name << "\n";
    std::string t = ctx.newTemp();
    ctx.emit(t + " = load i32, i32* %" + name);
    return t;
}

std::string ArgIntExpr::codegen(CodeGenContext& ctx) {
    std::string idx = index->codegen(ctx);
    std::string ptr = ctx.newTemp();
    std::string raw = ctx.newTemp();
    std::string val = ctx.newTemp();
    ctx.emit(ptr + " = getelementptr inbounds i8*, i8** %argv, i32 " + idx);
    ctx.emit(raw + " = load i8*, i8** " + ptr);
    ctx.emit(val + " = call i32 @atoi(i8* " + raw + ")");
    return val;
}

std::string BinaryExpr::codegen(CodeGenContext& ctx) {
    std::string l = lhs->codegen(ctx);
    std::string r = rhs->codegen(ctx);
    std::string t = ctx.newTemp();
    if (op == "+") ctx.emit(t + " = add i32 " + l + ", " + r);
    else if (op == "-") ctx.emit(t + " = sub i32 " + l + ", " + r);
    else if (op == "*") ctx.emit(t + " = mul i32 " + l + ", " + r);
    else if (op == "/") ctx.emit(t + " = sdiv i32 " + l + ", " + r);
    else if (op == "mod") ctx.emit(t + " = srem i32 " + l + ", " + r);
    else {
        std::string cmp;
        if (op == "=") cmp = "eq"; else if (op == "<>") cmp = "ne"; else if (op == "<") cmp = "slt";
        else if (op == "<=") cmp = "sle"; else if (op == ">") cmp = "sgt"; else if (op == ">=") cmp = "sge";
        std::string b = ctx.newTemp();
        ctx.emit(b + " = icmp " + cmp + " i32 " + l + ", " + r);
        ctx.emit(t + " = zext i1 " + b + " to i32");
    }
    return t;
}

void AssignStmt::codegen(CodeGenContext& ctx) {
    if (!ctx.symbols.exists(name)) std::cerr << "Erro semantico: variavel nao declarada: " << name << "\n";
    std::string v = expr->codegen(ctx);
    ctx.emit("store i32 " + v + ", i32* %" + name);
}

void WriteStmt::codegen(CodeGenContext& ctx) {
    for (auto& e : args) {
        if (auto* se = dynamic_cast<StringExpr*>(e.get())) {
            std::string p = se->codegen(ctx);
            ctx.emit("call i32 (i8*, ...) @printf(i8* " + p + ")");
        } else {
            std::string v = e->codegen(ctx);
            ctx.emit("call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.fmt_int, i32 0, i32 0), i32 " + v + ")");
        }
    }
    if (newline) ctx.emit("call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.fmt_nl, i32 0, i32 0))");
}

void IfStmt::codegen(CodeGenContext& ctx) {
    std::string c = cond->codegen(ctx);
    std::string b = ctx.newTemp();
    std::string thenL = ctx.newLabel("then");
    std::string elseL = ctx.newLabel("else");
    std::string endL = ctx.newLabel("endif");
    ctx.emit(b + " = icmp ne i32 " + c + ", 0");
    ctx.emit("br i1 " + b + ", label %" + thenL + ", label %" + elseL);
    ctx.emitLabel(thenL);
    for (auto& s : thenStmts) s->codegen(ctx);
    ctx.emit("br label %" + endL);
    ctx.emitLabel(elseL);
    for (auto& s : elseStmts) s->codegen(ctx);
    ctx.emit("br label %" + endL);
    ctx.emitLabel(endL);
}

void WhileStmt::codegen(CodeGenContext& ctx) {
    std::string condL = ctx.newLabel("whilecond");
    std::string bodyL = ctx.newLabel("whilebody");
    std::string endL = ctx.newLabel("whileend");
    ctx.emit("br label %" + condL);
    ctx.emitLabel(condL);
    std::string c = cond->codegen(ctx);
    std::string b = ctx.newTemp();
    ctx.emit(b + " = icmp ne i32 " + c + ", 0");
    ctx.emit("br i1 " + b + ", label %" + bodyL + ", label %" + endL);
    ctx.emitLabel(bodyL);
    for (auto& s : body) s->codegen(ctx);
    ctx.emit("br label %" + condL);
    ctx.emitLabel(endL);
}

void ForStmt::codegen(CodeGenContext& ctx) {
    std::string s = start->codegen(ctx);
    ctx.emit("store i32 " + s + ", i32* %" + var);
    std::string condL = ctx.newLabel("forcond");
    std::string bodyL = ctx.newLabel("forbody");
    std::string endL = ctx.newLabel("forend");
    ctx.emit("br label %" + condL);
    ctx.emitLabel(condL);
    std::string cur = ctx.newTemp();
    ctx.emit(cur + " = load i32, i32* %" + var);
    std::string endv = end->codegen(ctx);
    std::string cmp = ctx.newTemp();
    ctx.emit(cmp + " = icmp sle i32 " + cur + ", " + endv);
    ctx.emit("br i1 " + cmp + ", label %" + bodyL + ", label %" + endL);
    ctx.emitLabel(bodyL);
    for (auto& st : body) st->codegen(ctx);
    std::string next = ctx.newTemp();
    ctx.emit(next + " = add i32 " + cur + ", 1");
    ctx.emit("store i32 " + next + ", i32* %" + var);
    ctx.emit("br label %" + condL);
    ctx.emitLabel(endL);
}

void Program::codegen(CodeGenContext& ctx) {
    ctx.declareRuntime();
    ctx.ir << "define i32 @main(i32 %argc, i8** %argv) {\nentry:\n";
    (void)name;
    ctx.emit("%argc.addr = alloca i32");
    ctx.emit("store i32 %argc, i32* %argc.addr");
    for (const auto& v : variables) {
        ctx.symbols.declare(v);
        ctx.emit("%" + v + " = alloca i32");
        ctx.emit("store i32 0, i32* %" + v);
    }
    for (auto& s : statements) s->codegen(ctx);
    ctx.emit("ret i32 0");
    ctx.ir << "}\n";
}
