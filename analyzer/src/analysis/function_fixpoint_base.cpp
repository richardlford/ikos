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


bool FunctionFixpointBase::is_filtered_out(ar::Function* fun) const {
  auto opts = _ctx.opts;
  if (opts.functions_to_ignore.empty()) {
    return false; // No filtering.
  }
  std::string name = demangle(fun->name());
  bool result = (bool)std::regex_search(name, opts.functions_to_ignore_regex);
  std::cout << "is_filtered_out returns " << result << " for " << name << std::endl;
  return result;
}

} // end namespace analyzer
} // end namespace ikos
