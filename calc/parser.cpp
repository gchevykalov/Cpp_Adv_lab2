#include "parser.h"

/**
 * Send operators with higher priority from stack with operators to general stack
 * @param[in] op - current token
 */
void parser_t::displacementOperations(std::unique_ptr<token_t> op) {
  std::unique_ptr<token_t> tok;
  std::string err = "Incorrect argument in displasement operation";

  // verification of the received token
  if (op->type != token_t::token_type_t::TOKEN_TYPE_OPERATION)
    throw std::exception(err.c_str());

  token_operation_t* tmp = static_cast<token_operation_t*>(op.get());

  switch (tmp->operation->type) {
    case operation_t::operation_type_t::INFIX_OP:
      break;
    case operation_t::operation_type_t::POSTFIX_OP:
    {
      postfix_op_t* tmp1 = static_cast<postfix_op_t*>(tmp->operation.get());
      if (tmp1->postfixType == postfix_op_t::postfix_type_t::CLOSE_BRACKET)
        throw std::exception(err.c_str());
      break;
    }
    default:
      throw std::exception(err.c_str());
  }

  // crowding out higher priority operations
  while (!oper.empty()) {
    tok = std::move(oper.top());
    oper.pop();
    if (!checkPrior(static_cast<token_operation_t*>(tok.get()), tmp)) {
      oper.push(std::move(tok));
      break;
    }
    else
      gen.push(std::move(tok));
  }
  oper.push(std::move(op));
}

/**
 * Send operators from stack with operators to general stack until any open bracket
 * @return true if any open bracket was found
 */
bool parser_t::displacementUntilAnyOpenBracket() {
  std::unique_ptr<token_t> tok;
  token_operation_t* op;

  while (!oper.empty()) {
    tok = std::move(oper.top());
    oper.pop();
    op = static_cast<token_operation_t*>(tok.get());
    switch (op->operation->type) {
      case operation_t::operation_type_t::FUNCTION:
      case operation_t::operation_type_t::POSTFIX_OP:
      case operation_t::operation_type_t::INFIX_OP:
        gen.push(std::move(tok));
        break;
      default:
      {
        prefix_op_t* pref = static_cast<prefix_op_t*>(op->operation.get());
        
        if (pref->prefixType == prefix_op_t::prefix_type_t::PREFIX_OP)
          gen.push(std::move(tok));
        else
          return true;
        break;
      }
    }
  }

  return false;
}

/**
 * Send operators from stack with operators to general stack until open bracket with the same id
 * @param[in] op - current bracket token
 * @return true if open bracket with the same id was displacement
 */
bool parser_t::displacementUntilOpenBracket(std::unique_ptr<token_t> op) {
  std::unique_ptr<token_t> tok;
  token_operation_t* operation;
  close_bracket_t* cb;
  std::string err = "Incorrect argument in displasement until open bracket";

  // verification of the received token
  if (op->type != token_t::token_type_t::TOKEN_TYPE_OPERATION)
    throw std::exception(err.c_str());

  token_operation_t* tmp = static_cast<token_operation_t*>(op.get());

  if (tmp->operation->type != operation_t::operation_type_t::POSTFIX_OP)
    throw std::exception(err.c_str());

  postfix_op_t* tmp1 = static_cast<postfix_op_t*>(tmp->operation.get());

  if (tmp1->postfixType != postfix_op_t::postfix_type_t::CLOSE_BRACKET)
    throw std::exception(err.c_str());

  cb = static_cast<close_bracket_t*>(tmp1);

  // displacement to the corresponding bracket
  while (!oper.empty()) {
    tok = std::move(oper.top());
    oper.pop();
    operation = static_cast<token_operation_t*>(tok.get());
    switch (operation->operation->type) {
      case operation_t::operation_type_t::FUNCTION:
      case operation_t::operation_type_t::POSTFIX_OP:
      case operation_t::operation_type_t::INFIX_OP:
        gen.push(std::move(tok));
        break;
      default:
      {
        prefix_op_t* pref = static_cast<prefix_op_t*>(operation->operation.get());
      
        if (pref->prefixType == prefix_op_t::prefix_type_t::PREFIX_OP)
          gen.push(std::move(tok));
        else {
          open_bracket_t* ob = static_cast<open_bracket_t*>(pref);

          if (ob->pairID == cb->pairID) {
            gen.push(std::move(tok));
            gen.push(std::move(op));
            return true;
          }
          else // situation like ({)
            return false;
        }
        break;
      }
    }
  }

  return false;
}

/**
 * Check which of operators has higher priority
 * @param[in] op1 - first token
 * @param[in] op2 - second token
 * @return true if op1 has higher priority than op2
 */
bool parser_t::checkPrior(token_operation_t* op1, token_operation_t* op2) {
  if (op1->operation->type == operation_t::operation_type_t::FUNCTION ||
        op1->operation->type == operation_t::operation_type_t::POSTFIX_OP)
    return true; // functions and postfix operations are always replaced

  if (op1->operation->type == operation_t::operation_type_t::INFIX_OP) {
    if (op2->operation->type == operation_t::operation_type_t::INFIX_OP) {
      infix_t* tmpop1 = static_cast<infix_t*>(op1->operation.get());
      
      // the displacement of infix operations is determined by priority and associativity
      if (tmpop1->assoc == infix_t::operation_assoc_t::TO_RIGHT)
        return tmpop1->prior >= op2->operation.get()->prior;
      else
        return tmpop1->prior > op2->operation.get()->prior;
    }
    else
      return false;
  }
  else {
    prefix_op_t* tmpop1 = static_cast<prefix_op_t*>(op1->operation.get());

    // the opening brackets are not displaced
    if (tmpop1->prefixType == prefix_op_t::prefix_type_t::OPEN_BRACKET)
      return false;
    else
      return tmpop1->prior > op2->operation.get()->prior;
  }
}

/**
 * Processes the current token in the parser state STATE_OPERAND
 * @param[in] op - current token
 * @return state after processing
 */
parser_t::state_t parser_t::processOperand(std::unique_ptr<token_t> op) {
  state_t state = state_t::STATE_OPERATION;

  switch (op->type) {
    case token_t::token_type_t::TOKEN_TYPE_NUMBER:
    case token_t::token_type_t::TOKEN_TYPE_VARIABLE:
      gen.push(std::move(op));
      break;
    default:
      token_operation_t* tok = static_cast<token_operation_t*>(op.get());

      switch (tok->operation->type) {
        case operation_t::operation_type_t::FUNCTION:
        case operation_t::operation_type_t::PREFIX_OP:
          oper.push(std::move(op));
          state = state_t::STATE_OPERAND;
          break;
        default: // at this stage, it is not expected to encounter an infix or postfix operation
          throw std::exception("Unexpected operation");
      }
      break;
  }

  return state;
}

/**
 * Processes the current token in the parser state STATE_OPERATION
 * @param[in] op - current token
 * @return state after processing
 */
parser_t::state_t parser_t::processOperation(std::unique_ptr<token_t> op) {
  state_t state = state_t::STATE_OPERAND;

  if (op->type == token_t::token_type_t::TOKEN_TYPE_OPERATION) {
    token_operation_t* tok = static_cast<token_operation_t*>(op.get());

    switch (tok->operation->type) {
      case operation_t::operation_type_t::INFIX_OP:
        displacementOperations(std::move(op));
        break;
      case operation_t::operation_type_t::POSTFIX_OP:
      {
        postfix_op_t* postf = static_cast<postfix_op_t*>(tok->operation.get());

        if (postf->postfixType == postfix_op_t::postfix_type_t::POSTFIX_OP)
          displacementOperations(std::move(op));
        else // due to the special behavior, the brackets are handled separately
          if (!displacementUntilOpenBracket(std::move(op)))
            throw std::exception("Error with brackets");
        state = state_t::STATE_OPERATION;
        break;
      }
      default: // at this stage, only postfix and infix operations are expected
        throw std::exception("Unexpected operation");
    }
  }
  else // at this stage, only postfix and infix operations are expected
    throw std::exception("Unexpected operand");

  return state;
}

/**
 * Clear resulting queue, general stack and intermediate stack
 */
void parser_t::clear() {
  while (!qres.empty())
    qres.pop();
  while (!gen.empty())
    gen.pop();
  while (!oper.empty())
    oper.pop();
}

/**
 * Parse queue and transform it to rpn
 * @param[in] tokens - queue of tokens
 * @return queue of tokens in RPN
 */
token_queue_t& parser_t::parse(token_queue_t& tokens) {
  state_t state = state_t::STATE_OPERAND;
  std::unique_ptr<token_t> tok;

  clear();

  // process all tokens
  while (!tokens.empty()) {
    tok = std::move(tokens.front());
    tokens.pop();

    if (state == state_t::STATE_OPERAND) {
      state = processOperand(std::move(tok));
    }
    else {
      state = processOperation(std::move(tok));
    }
  }

  if (state == state_t::STATE_OPERAND)
    throw std::exception("Unexpected end");

  if(displacementUntilAnyOpenBracket())
    throw std::exception("Missing a closing bracket");

  // moving to the resulting queue
  while (!gen.empty()) {
    tok = std::move(gen.top());
    gen.pop();
    oper.push(std::move(tok));
  }
  while (!oper.empty()) {
    tok = std::move(oper.top());
    oper.pop();
    qres.push(std::move(tok));
  }

  return qres;
}