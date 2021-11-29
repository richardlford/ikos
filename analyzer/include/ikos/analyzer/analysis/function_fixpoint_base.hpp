//
// Created by fordrl on 11/23/21.
//

#ifndef IKOS_FUNCTION_FIXPOINT_BASE_HPP
#define IKOS_FUNCTION_FIXPOINT_BASE_HPP

#include <ikos/ar/semantic/code.hpp>
#include <ikos/ar/semantic/function.hpp>

#include <ikos/core/fixpoint/fwd_fixpoint_iterator.hpp>

#include <ikos/analyzer/util/demangle.hpp>
#include <ikos/analyzer/analysis/call_context.hpp>
#include <ikos/analyzer/analysis/context.hpp>
#include <ikos/analyzer/analysis/execution_engine/fixpoint_cache.hpp>
#include <ikos/analyzer/analysis/fixpoint_parameters.hpp>
#include <ikos/analyzer/analysis/value/abstract_domain.hpp>

namespace ikos {
namespace analyzer {

class FunctionFixpointBase {

private:
  /// \brief Analysis context
  Context& _ctx;

  /// \brief Analyzed function
  ar::Function* _function;

public:
  /// \brief Constructor for an entry point
  ///
  /// \param ctx Analysis context
  /// \param checkers List of checkers to run
  /// \param entry_point Function to analyze
  FunctionFixpointBase(Context& ctx, ar::Function* entry_point) noexcept
      : _ctx(ctx), _function(entry_point)
  {

  }

  /// \brief Return true if checks for this function should be filtered out.
  ///
  /// \param fun The function to check
  /// \return True if the functions checks should be omitted.
  bool is_filtered_out(ar::Function* fun) const;
};

} // end namespace analyzer
} // end namespace ikos

#endif // IKOS_FUNCTION_FIXPOINT_BASE_HPP
