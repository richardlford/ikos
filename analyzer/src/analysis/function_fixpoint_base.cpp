//
// Created by fordrl on 11/23/21.
//
#include <ikos/ar/semantic/code.hpp>
#include <ikos/ar/semantic/function.hpp>

#include <ikos/core/fixpoint/fwd_fixpoint_iterator.hpp>

#include <ikos/analyzer/util/demangle.hpp>
#include <ikos/analyzer/analysis/call_context.hpp>
#include <ikos/analyzer/analysis/context.hpp>
#include <ikos/analyzer/analysis/execution_engine/fixpoint_cache.hpp>
#include <ikos/analyzer/analysis/fixpoint_parameters.hpp>
#include <ikos/analyzer/analysis/value/abstract_domain.hpp>

#include <ikos/analyzer/analysis/function_fixpoint_base.hpp>

namespace ikos {
namespace analyzer {

namespace {

std::string ops[] = {
    "->*", "<=>", "<<=", ">>=", // Length 3
    "()", "->", "<=", ">=", "<<", ">>", // Length 2
    "<", ">"
};

/// \brief Removing leading return type from a function's demangled name.
std::string remove_return_type(std::string name) {
  // The strategy is to look for space outside balanced ()<>.
  // A tricky issue is what to do for operator << or >>.

  // Nesting depth of () or <>.
  int depth = 0;

  auto len = name.length();
  decltype(len) i = 0;
  while (i < len) {
    char ch = name[i++];
    if (ch == ' ' && depth == 0) {
      while (i < len && name[i] == ' ') {
        i++;
      }
      // Skip possible trailing "const".
      if (((i+4) < len) && (name.substr(i, 5) == "const")) {
        i += 5;
        while (i < len && name[i] == ' ') {
          i++;
        }
      }
      if (i == len) {
        // The blank was found at the end. Just return input.
        return name;
      }
      auto result = name.substr(i);
      return result;
    }
    // Need to advance and adjust depth.
    if (ch == '(') {
      depth++;
    }else if (ch == ')') {
      depth--;
    } else if (ch == '<') {
      depth++;
    }else if (ch == '>') {
      depth--;
    } else if (ch == 'o' && ((i + 6) < len) &&
               name.substr(i, 7) == "perator") {
      // Matching "operator".
      i += 7;
      // Skip blanks
      while (i < len && name[i] == ' ') {
        i++;
      }

      // Match and skip over operators that might have given a false
      // depth count.
      for (auto& op: ops) {
        auto op_len = op.length();
        if (((i + op_len - 1) < len) && (name.substr(i, op_len) == op)) {
          i += op_len;
          break;
        }
      }
    }
  }

  // We didn't find a blank outside nested delimiters, so just return
  // the complete name.
  return name;
}
}

bool FunctionFixpointBase::is_filtered_out(ar::Function* fun) const {
  auto opts = _ctx.opts;
  if (opts.functions_to_ignore.empty()) {
    return false; // No filtering.
  }
  std::string name_optionally_with_return_type = demangle(fun->name());
  std::string name = remove_return_type(name_optionally_with_return_type);
  bool result = (bool)std::regex_search(name, opts.functions_to_ignore_regex);
  // std::cout << "is_filtered_out returns " << result << " for " << name << std::endl;
  return result;
}

} // end namespace analyzer
} // end namespace ikos
