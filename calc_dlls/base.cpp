#include "include/operation.h"
#include <cmath>

class Plus : public infix_t {
public:
  Plus() : infix_t(1.0, infix_t::operation_assoc_t::TO_RIGHT) {}

  ~Plus() = default;

  void process(token_stack_t& stack) override {
    double b = getNumber(stack);
    double a = getNumber(stack);

    stack.push(std::unique_ptr<token_number_t>(new token_number_t(a + b)));
  }
};

class Minus : public infix_t {
public:
  Minus() : infix_t(1.0, infix_t::operation_assoc_t::TO_RIGHT) {}

  ~Minus() = default;

  void process(token_stack_t& stack) override {
    double b = getNumber(stack);
    double a = getNumber(stack);

    stack.push(std::unique_ptr<token_number_t>(new token_number_t(a - b)));
  }
};

class Mul : public infix_t {
public:
  Mul() : infix_t(2.0, infix_t::operation_assoc_t::TO_RIGHT) {}

  ~Mul() = default;

  void process(token_stack_t& stack) override {
    double b = getNumber(stack);
    double a = getNumber(stack);

    stack.push(std::unique_ptr<token_number_t>(new token_number_t(a * b)));
  }
};

class Div : public infix_t {
public:
  Div() : infix_t(2.0, infix_t::operation_assoc_t::TO_RIGHT) {}

  ~Div() = default;

  void process(token_stack_t& stack) override {
    double b = getNumber(stack);
    double a = getNumber(stack);

    stack.push(std::unique_ptr<token_number_t>(new token_number_t(a / b)));
  }
};

class UnarMinus : public prefix_t {
public:
  UnarMinus() : prefix_t(4) {};

  ~UnarMinus() = default;

  void process(token_stack_t& stack) override {
    double a = getNumber(stack);

    stack.push(std::unique_ptr<token_number_t>(new token_number_t(-a)));
  }
};

class OpenBracket : public open_bracket_t {
public:
  OpenBracket() : open_bracket_t(1) {}

  ~OpenBracket() = default;
};

class CloseBracket : public close_bracket_t {
public:
  CloseBracket() : close_bracket_t(1) {}

  ~CloseBracket() = default;

  void process(token_stack_t& stack) override {}
};

extern "C" __declspec(dllexport) void __cdecl load(ops_maps & m, std::map<std::string, double const>&cv) {
  m.inf.insert(std::make_pair("+", std::shared_ptr<infix_t>(new Plus)));
  m.inf.insert(std::make_pair("-", std::shared_ptr<infix_t>(new Minus)));
  m.inf.insert(std::make_pair("*", std::shared_ptr<infix_t>(new Mul)));
  m.inf.insert(std::make_pair("/", std::shared_ptr<infix_t>(new Div)));
  m.pref.insert(std::make_pair("-", std::shared_ptr<prefix_t>(new UnarMinus)));
  m.pref.insert(std::make_pair("(", std::shared_ptr<open_bracket_t>(new OpenBracket)));
  m.postf.insert(std::make_pair(")", std::shared_ptr<close_bracket_t>(new CloseBracket)));
}