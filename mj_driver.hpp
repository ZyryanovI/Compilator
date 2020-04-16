#pragma once

#include <cassert>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <istream>
#include <string>

#include "mj_scanner.hpp"
// #include "mj_parser.tab.hh"
#include "ast/handlers/expressions.hpp"
#include "ast/handlers/statements.hpp"
#include "ast/visitors/visitor_pretty_printer.hpp"
#include "ast/visitors/visitor_graphviz.hpp"

namespace MJ {

typedef std::shared_ptr<ast::Expression> PExpression;
typedef std::shared_ptr<ast::ExpressionInt> PExpressionInt;
typedef std::shared_ptr<ast::ExpressionBinaryOp> PExpressionBinaryOp;

class MJ_Driver{
public:
   MJ_Driver() = default;

   virtual ~MJ_Driver();

   // reading from file
   void parse(const char * const input_filename, const char * const output_filename);
private:

   void parse_helper(std::istream &i_stream, std::ofstream &o_stream);
   void parse_tree(PExpression new_root);
   MJ::MJ_Parser  *parser  = nullptr;
   MJ::MJ_Scanner *scanner = nullptr;
};

}
