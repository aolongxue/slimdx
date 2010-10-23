// Copyright (c) 2007-2010 SlimDX Group
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
#if !defined(BOOST_WAVE_ADVANCED_PREPROCESSING_HOOKS_INCLUDED)
#define BOOST_WAVE_ADVANCED_PREPROCESSING_HOOKS_INCLUDED

#include <cstdio>
#include <ostream>
#include <string>

#include <boost/assert.hpp>
#include <boost/config.hpp>

#include <boost/wave/token_ids.hpp>
#include <boost/wave/util/macro_helpers.hpp>
#include <boost/wave/preprocessing_hooks.hpp>

#include "BoostWaveNative.h"

///////////////////////////////////////////////////////////////////////////////
//  
//  The advanced_preprocessing_hooks policy class is used to register some
//  of the more advanced (and probably more rarely used hooks with the Wave
//  library.
//
//  This policy type is used as a template parameter to the boost::wave::context<>
//  object.
//
///////////////////////////////////////////////////////////////////////////////
class advanced_preprocessing_hooks
	:   public boost::wave::context_policies::default_preprocessing_hooks
{
public:
	ProcessCallback* callback;

	advanced_preprocessing_hooks() {}

	/*
	template <typename ContextT, typename TokenT>
	TokenT const&
		generated_token(ContextT const& ctx, TokenT const& t)
	{ 
		//using namespace boost::wave;
		//token_id id = token_id(t);
		//token_category tokenCategory = (token_category)CATEGORY_FROM_TOKEN(id);
		//std::cout << "Token: Type [" << get_token_name(id) << "] [";
		//switch (tokenCategory) {
		//	case EOLTokenType:
		//		std::cout << "EOL";
		//		break;
		//	case KeywordTokenType:
		//		std::cout << "Keyword";
		//		break;
		//	case OperatorTokenType:
		//		std::cout << "Operator";
		//		break;
		//	default:
		//		break;
		//}

		if ( !t.is_eoi() ) {
			std::cout << "] Value [" << t.get_value().c_str() << "]" << std::endl;
		}

		return t; 
	}
	
	// new signature
	template <typename ContextT, typename TokenT, typename ContainerT, typename IteratorT>
	bool 
		expanding_function_like_macro(ContextT const& ctx,
		TokenT const& macrodef, std::vector<TokenT> const& formal_args, 
		ContainerT const& definition,
		TokenT const& macrocall, std::vector<ContainerT> const& arguments,
		IteratorT const& seqstart, IteratorT const& seqend) 
	{ 
		  

		  std::cout << "// Macro expansion here" << std::endl;
		  std::cout << "// MacroCall : [" << macrocall.get_value().c_str() << "]" << std::endl;

		  // Display args
		  for(int i = 0; i < arguments.size(); i++) {
			  
			  std::cout << "Arg: [" << boost::wave::util::impl::as_string(arguments[i]) << "]" << std::endl;
		  }


		return true; 
	}   // default is to normally expand the macro
	*/

    ///////////////////////////////////////////////////////////////////////////
    //  
    //  The function 'expanding_function_like_macro' is called, whenever a 
    //  function-like macro is to be expanded.
    //
    //  The parameter 'macrodef' marks the position, where the macro to expand 
    //  is defined.
    //
    //  The parameter 'formal_args' holds the formal arguments used during the
    //  definition of the macro.
    //
    //  The parameter 'definition' holds the macro definition for the macro to 
    //  trace.
    //
    //  The parameter 'macro_call' marks the position, where this macro invoked.
    //
    //  The parameter 'arguments' holds the macro arguments used during the 
    //  invocation of the macro
    //
    //  The parameters 'seqstart' and 'seqend' point into the input token 
    //  stream allowing to access the whole token sequence comprising the macro
    //  invocation (starting with the opening parenthesis and ending after the
    //  closing one).
    //
    //  The return value defines, whether the corresponding macro will be 
    //  expanded (return false) or will be copied to the output (return true).
    //  Note: the whole argument list is copied unchanged to the output as well
    //        without any further processing.
    //
    ///////////////////////////////////////////////////////////////////////////

    // new signature
    template <typename ContextT, typename TokenT, typename ContainerT, typename IteratorT>
    bool 
    expanding_function_like_macro(ContextT const& ctx,
        TokenT const& macrodef, std::vector<TokenT> const& formal_args, 
        ContainerT const& definition,
        TokenT const& macrocall, std::vector<ContainerT> const& arguments,
        IteratorT const& seqstart, IteratorT const& seqend) 
    { 
		

		callback->MacroFunctionCall(macrocall.get_value().c_str());

		for(int i = 0; i < arguments.size(); i++) {
			callback->MacroArg();

			ContainerT arg = arguments[i];

			ContainerT::const_iterator begin_it = arg.begin();
			ContainerT::const_iterator end_it = arg.end();

			while (begin_it != end_it) {
				TokenT token = *begin_it;
				boost::wave::token_id id = boost::wave::token_id(token);

				callback->Token(id, token.get_value().c_str() );

				begin_it++;
			}
		}		

		callback->MacroFunctionEnd();
		
		return false; 
	
	
	}   // default is to normally expand the macro

    ///////////////////////////////////////////////////////////////////////////
    //  
    //  The function 'expanding_object_like_macro' is called, whenever a 
    //  object-like macro is to be expanded .
    //
    //  The parameter 'ctx' is a reference to the context object used for 
    //  instantiating the preprocessing iterators by the user.
    //
    //  The parameter 'macro' marks the position, where the macro to expand 
    //  is defined.
    //
    //  The definition 'definition' holds the macro definition for the macro to 
    //  trace.
    //
    //  The parameter 'macrocall' marks the position, where this macro invoked.
    //
    //  The return value defines, whether the corresponding macro will be 
    //  expanded (return false) or will be copied to the output (return true).
    //
    ///////////////////////////////////////////////////////////////////////////
    // new signature
    template <typename ContextT, typename TokenT, typename ContainerT>
    bool 
    expanding_object_like_macro(ContextT const& ctx, TokenT const& macro, 
        ContainerT const& definition, TokenT const& macrocall)
    { return false; }   // default is to normally expand the macro

    ///////////////////////////////////////////////////////////////////////////
    //  
    //  The function 'expanded_macro' is called, whenever the expansion of a 
    //  macro is finished but before the rescanning process starts.
    //
    //  The parameter 'ctx' is a reference to the context object used for 
    //  instantiating the preprocessing iterators by the user.
    //
    //  The parameter 'result' contains the token sequence generated as the 
    //  result of the macro expansion.
    //
    ///////////////////////////////////////////////////////////////////////////
    // new signature
    template <typename ContextT, typename ContainerT>
    void expanded_macro(ContextT const& ctx, ContainerT const& result)
    {}

    ///////////////////////////////////////////////////////////////////////////
    //  
    //  The function 'rescanned_macro' is called, whenever the rescanning of a 
    //  macro is finished.
    //
    //  The parameter 'ctx' is a reference to the context object used for 
    //  instantiating the preprocessing iterators by the user.
    //
    //  The parameter 'result' contains the token sequence generated as the 
    //  result of the rescanning.
    //
    ///////////////////////////////////////////////////////////////////////////
    // new signature
    template <typename ContextT, typename ContainerT>
    void rescanned_macro(ContextT const& ctx, ContainerT const& result)
    {}

    ///////////////////////////////////////////////////////////////////////////
    //  
    //  The function 'found_include_directive' is called, whenever a #include
    //  directive was located.
    //
    //  The parameter 'ctx' is a reference to the context object used for 
    //  instantiating the preprocessing iterators by the user.
    //
    //  The parameter 'filename' contains the (expanded) file name found after 
    //  the #include directive. This has the format '<file>', '"file"' or 
    //  'file'.
    //  The formats '<file>' or '"file"' are used for #include directives found 
    //  in the preprocessed token stream, the format 'file' is used for files
    //  specified through the --force_include command line argument.
    //
    //  The parameter 'include_next' is set to true if the found directive was
    //  a #include_next directive and the BOOST_WAVE_SUPPORT_INCLUDE_NEXT
    //  preprocessing constant was defined to something != 0.
    //
    //  The return value defines, whether the found file will be included 
    //  (return false) or will be skipped (return true).
    //
    ///////////////////////////////////////////////////////////////////////////
    // new signature
    template <typename ContextT>
    bool 
    found_include_directive(ContextT const& ctx, std::string const& filename, 
        bool include_next) 
    {
        return false;    // ok to include this file
    }
    
    ///////////////////////////////////////////////////////////////////////////
    //  
    //  The function 'opened_include_file' is called, whenever a file referred 
    //  by an #include directive was successfully located and opened.
    //
    //  The parameter 'ctx' is a reference to the context object used for 
    //  instantiating the preprocessing iterators by the user.
    //
    //  The parameter 'filename' contains the file system path of the 
    //  opened file (this is relative to the directory of the currently 
    //  processed file or a absolute path depending on the paths given as the
    //  include search paths).
    //
    //  The include_depth parameter contains the current include file depth.
    //
    //  The is_system_include parameter denotes, whether the given file was 
    //  found as a result of a #include <...> directive.
    //  
    ///////////////////////////////////////////////////////////////////////////
    // new signature
    template <typename ContextT>
    void 
    opened_include_file(ContextT const& ctx, std::string const& relname, 
        std::string const& absname, bool is_system_include) 
    {
		callback->IncludeBegin(relname.c_str());
	}

    ///////////////////////////////////////////////////////////////////////////
    //  
    //  The function 'returning_from_include_file' is called, whenever an
    //  included file is about to be closed after it's processing is complete.
    //
    //  The parameter 'ctx' is a reference to the context object used for 
    //  instantiating the preprocessing iterators by the user.
    //
    ///////////////////////////////////////////////////////////////////////////
    // new signature
    template <typename ContextT>
    void
    returning_from_include_file(ContextT const& ctx) 
    {
		callback->IncludeEnd();
	}


#if BOOST_WAVE_SUPPORT_PRAGMA_ONCE != 0
    ///////////////////////////////////////////////////////////////////////////
    //  
    //  The function 'detected_include_guard' is called whenever either a 
    //  include file is about to be added to the list of #pragma once headers.
    //  That means this header file will not be opened and parsed again even 
    //  if it is specified in a later #include directive.
    //  This function is called as the result of a detected include guard 
    //  scheme. 
    //
    //  The implemented heuristics for include guards detects two forms of 
    //  include guards:
    // 
    //       #ifndef INCLUDE_GUARD_MACRO
    //       #define INCLUDE_GUARD_MACRO
    //       ...
    //       #endif
    // 
    //   or
    // 
    //       if !defined(INCLUDE_GUARD_MACRO)
    //       #define INCLUDE_GUARD_MACRO
    //       ...
    //       #endif
    // 
    //  note, that the parenthesis are optional (i.e. !defined INCLUDE_GUARD_MACRO
    //  will work as well). The code allows for any whitespace, newline and single 
    //  '#' tokens before the #if/#ifndef and after the final #endif.
    //
    //  The parameter 'ctx' is a reference to the context object used for 
    //  instantiating the preprocessing iterators by the user.
    //
    //  The parameter 'filename' contains the file system path of the 
    //  opened file (this is relative to the directory of the currently 
    //  processed file or a absolute path depending on the paths given as the
    //  include search paths).
    //
    //  The parameter contains the name of the detected include guard.
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename ContextT>
    void
    detected_include_guard(ContextT const& ctx, std::string const& filename,
        std::string const& include_guard) 
    {}

    ///////////////////////////////////////////////////////////////////////////
    //  
    //  The function 'detected_pragma_once' is called whenever either a 
    //  include file is about to be added to the list of #pragma once headers.
    //  That means this header file will not be opened and parsed again even 
    //  if it is specified in a later #include directive.
    //  This function is called as the result of a detected directive
    //  #pragma once. 
    //  
    //  The parameter 'ctx' is a reference to the context object used for 
    //  instantiating the preprocessing iterators by the user.
    //
    //  The parameter pragma_token refers to the token "#pragma" triggering 
    //  this preprocessing hook.
    //
    //  The parameter 'filename' contains the file system path of the 
    //  opened file (this is relative to the directory of the currently 
    //  processed file or a absolute path depending on the paths given as the
    //  include search paths).
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename ContextT, typename TokenT>
    void
    detected_pragma_once(ContextT const& ctx, TokenT const& pragma_token,
        std::string const& filename) 
    {}
#endif 

    ///////////////////////////////////////////////////////////////////////////
    //  
    //  The function 'interpret_pragma' is called, whenever a '#pragma command' 
    //  directive is found which isn't known to the core Wave library, where
    //  'command' is the value defined as the BOOST_WAVE_PRAGMA_KEYWORD constant
    //  which defaults to "wave".
    //
    //  The parameter 'ctx' is a reference to the context object used for 
    //  instantiating the preprocessing iterators by the user.
    //
    //  The parameter 'pending' may be used to push tokens back into the input 
    //  stream, which are to be used as the replacement text for the whole 
    //  #pragma directive.
    //
    //  The parameter 'option' contains the name of the interpreted pragma.
    //
    //  The parameter 'values' holds the values of the parameter provided to 
    //  the pragma operator.
    //
    //  The parameter 'act_token' contains the actual #pragma token, which may 
    //  be used for error output.
    //
    //  If the return value is 'false', the whole #pragma directive is 
    //  interpreted as unknown and a corresponding error message is issued. A
    //  return value of 'true' signs a successful interpretation of the given 
    //  #pragma.
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename ContextT, typename ContainerT>
    bool 
    interpret_pragma(ContextT const& ctx, ContainerT &pending, 
        typename ContextT::token_type const& option, ContainerT const& values, 
        typename ContextT::token_type const& act_token)
    {
        return false;
    }

    ///////////////////////////////////////////////////////////////////////////
    //
    //  The function 'emit_line_directive' is called whenever a #line directive
    //  has to be emitted into the generated output.
    //
    //  The parameter 'ctx' is a reference to the context object used for 
    //  instantiating the preprocessing iterators by the user.
    //
    //  The parameter 'pending' may be used to push tokens back into the input 
    //  stream, which are to be used instead of the default output generated
    //  for the #line directive.
    //
    //  The parameter 'act_token' contains the actual #pragma token, which may 
    //  be used for error output. The line number stored in this token can be
    //  used as the line number emitted as part of the #line directive.
    //
    //  If the return value is 'false', a default #line directive is emitted
    //  by the library. A return value of 'true' will inhibit any further 
    //  actions, the tokens contained in 'pending' will be copied verbatim 
    //  to the output.
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename ContextT, typename ContainerT>
    bool 
    emit_line_directive(ContextT const& ctx, ContainerT &pending, 
        typename ContextT::token_type const& act_token)
    {
        return false;
    }

    ///////////////////////////////////////////////////////////////////////////
    //
    //  The function 'defined_macro' is called, whenever a macro was defined
    //  successfully.
    //
    //  The parameter 'ctx' is a reference to the context object used for 
    //  instantiating the preprocessing iterators by the user.
    //
    //  The parameter 'name' is a reference to the token holding the macro name.
    //
    //  The parameter 'is_functionlike' is set to true, whenever the newly 
    //  defined macro is defined as a function like macro.
    //
    //  The parameter 'parameters' holds the parameter tokens for the macro
    //  definition. If the macro has no parameters or if it is a object like
    //  macro, then this container is empty.
    //
    //  The parameter 'definition' contains the token sequence given as the
    //  replacement sequence (definition part) of the newly defined macro.
    //
    //  The parameter 'is_predefined' is set to true for all macros predefined 
    //  during the initialization phase of the library.
    //
    ///////////////////////////////////////////////////////////////////////////
    // new signature
    template <
        typename ContextT, typename TokenT, typename ParametersT, 
        typename DefinitionT
    >
    void
    defined_macro(ContextT const& ctx, TokenT const& macro_name, 
        bool is_functionlike, ParametersT const& parameters, 
        DefinitionT const& definition, bool is_predefined)
    {
		callback->MacroDefBegin(macro_name.get_value().c_str(), parameters.size());

		// std::cout << "#MACRO_DEF_START " << macro_name.get_value().c_str() << "," << is_functionlike << "," << is_predefined;

		// Put Parameters
		if ( parameters.size() > 0 ) {
			for(int i = 0; i < parameters.size(); i++) {
				TokenT token = parameters[i];
				boost::wave::token_id id = boost::wave::token_id(token);

				callback->Token(id, token.get_value().c_str());
			}
		}

		callback->MacroDefBody();

		// Display args
		DefinitionT::const_iterator begin_it = definition.begin();
		DefinitionT::const_iterator end_it = definition.end();

		while (begin_it != end_it) {
			TokenT token = *begin_it;
			boost::wave::token_id id = boost::wave::token_id(token);

			callback->Token(id, token.get_value().c_str());
			begin_it++;
		}

		callback->MacroDefEnd();
	}

    ///////////////////////////////////////////////////////////////////////////
    //
    //  The function 'undefined_macro' is called, whenever a macro definition
    //  was removed successfully.
    //  
    //  The parameter 'ctx' is a reference to the context object used for 
    //  instantiating the preprocessing iterators by the user.
    //
    //  The parameter 'name' holds the name of the macro, which definition was 
    //  removed.
    //
    ///////////////////////////////////////////////////////////////////////////
    // new signature
    template <typename ContextT, typename TokenT>
    void
    undefined_macro(ContextT const& ctx, TokenT const& macro_name)
    {

		callback->MacroUnDef(macro_name.get_value().c_str());
	}

    ///////////////////////////////////////////////////////////////////////////
    //
    //  The function 'found_directive' is called, whenever a preprocessor 
    //  directive was encountered, but before the corresponding action is 
    //  executed.
    //
    //  The parameter 'ctx' is a reference to the context object used for 
    //  instantiating the preprocessing iterators by the user.
    //
    //  The parameter 'directive' is a reference to the token holding the 
    //  preprocessing directive.
    //
    //  The return value defines, whether the given expression has to be 
    //  to be executed in a normal way (return 'false'), or if it has to be  
    //  skipped altogether (return 'true'), which means it gets replaced in the 
    //  output by a single newline.
    //
    ///////////////////////////////////////////////////////////////////////////
    // new signature
    template <typename ContextT, typename TokenT>
    bool
    found_directive(ContextT const& ctx, TokenT const& directive)
    { return false; }   // by default we never skip any directives

    ///////////////////////////////////////////////////////////////////////////
    //
    //  The function 'found_unknown_directive' is called, whenever an unknown 
    //  preprocessor directive was encountered.
    //
    //  The parameter 'ctx' is a reference to the context object used for 
    //  instantiating the preprocessing iterators by the user.
    //
    //  The parameter 'line' holds the tokens of the entire source line
    //  containing the unknown directive.
    //
    //  The parameter 'pending' may be used to push tokens back into the input 
    //  stream, which are to be used as the replacement text for the whole 
    //  line containing the unknown directive.
    //
    //  The return value defines, whether the given expression has been 
    //  properly interpreted by the hook function or not. If this function 
    //  returns 'false', the library will raise an 'ill_formed_directive' 
    //  preprocess_exception. Otherwise the tokens pushed back into 'pending'
    //  are passed on to the user program.
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename ContextT, typename ContainerT>
    bool
    found_unknown_directive(ContextT const& ctx, ContainerT const& line, 
        ContainerT& pending)
    { return false; }   // by default we never interpret unknown directives

    ///////////////////////////////////////////////////////////////////////////
    //
    //  The function 'evaluated_conditional_expression' is called, whenever a 
    //  conditional preprocessing expression was evaluated (the expression
    //  given to a #if, #elif, #ifdef or #ifndef directive)
    //
    //  The parameter 'ctx' is a reference to the context object used for 
    //  instantiating the preprocessing iterators by the user.
    //
    //  The parameter 'directive' is a reference to the token holding the 
    //  corresponding preprocessing directive.
    //
    //  The parameter 'expression' holds the non-expanded token sequence
    //  comprising the evaluated expression.
    //
    //  The parameter expression_value contains the result of the evaluation of
    //  the expression in the current preprocessing context.
    //
    //  The return value defines, whether the given expression has to be 
    //  evaluated again, allowing to decide which of the conditional branches
    //  should be expanded. You need to return 'true' from this hook function 
    //  to force the expression to be re-evaluated.
    //
    ///////////////////////////////////////////////////////////////////////////
    // new signature
    template <typename ContextT, typename TokenT, typename ContainerT>
    bool
    evaluated_conditional_expression(ContextT const& ctx, 
        TokenT const& directive, ContainerT const& expression, 
        bool expression_value)
    { return false; }         // ok to continue, do not re-evaluate expression

    ///////////////////////////////////////////////////////////////////////////
    //
    //  The function 'skipped_token' is called, whenever a token is about to be
    //  skipped due to a false preprocessor condition (code fragments to be
    //  skipped inside the not evaluated conditional #if/#else/#endif branches).
    //
    //  The parameter 'ctx' is a reference to the context object used for 
    //  instantiating the preprocessing iterators by the user.
    //
    //  The parameter 'token' refers to the token to be skipped.
    //
    ///////////////////////////////////////////////////////////////////////////
    // new signature
    template <typename ContextT, typename TokenT>
    void
    skipped_token(ContextT const& ctx, TokenT const& token)
    {}

    ///////////////////////////////////////////////////////////////////////////
    //
    //  The function 'generated_token' will be called by the library whenever a
    //  token is about to be returned from the library.
    //
    //  The parameter 'ctx' is a reference to the context object used for 
    //  instantiating the preprocessing iterators by the user.
    //
    //  The parameter 't' is the token about to be returned from the library.
    //  This function may alter the token, but in this case it must be 
    //  implemented with a corresponding signature: 
    //
    //      TokenT const&
    //      generated_token(ContextT const& ctx, TokenT& t);
    //
    //  which makes it possible to modify the token in place.
    //
    //  The default behavior is to return the token passed as the parameter 
    //  without modification.
    //  
    ///////////////////////////////////////////////////////////////////////////
    template <typename ContextT, typename TokenT>
    TokenT const&
    generated_token(ContextT const& ctx, TokenT const& t)
    { return t; }

    ///////////////////////////////////////////////////////////////////////////
    //
    //  The function 'may_skip_whitespace' will be called by the 
    //  library, whenever it must be tested whether a specific token refers to 
    //  whitespace and this whitespace has to be skipped.
    //
    //  The parameter 'ctx' is a reference to the context object used for 
    //  instantiating the preprocessing iterators by the user.
    //
    //  The 'token' parameter holds a reference to the current token. The policy 
    //  is free to change this token if needed.
    //
    //  The 'skipped_newline' parameter holds a reference to a boolean value 
    //  which should be set to true by the policy function whenever a newline 
    //  is going to be skipped. 
    //
    //  If the return value is true, the given token is skipped and the 
    //  preprocessing continues to the next token. If the return value is 
    //  false, the given token is returned to the calling application. 
    //
    //  ATTENTION!
    //  Caution has to be used, because by returning true the policy function 
    //  is able to force skipping even significant tokens, not only whitespace. 
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename ContextT, typename TokenT>
    bool
    may_skip_whitespace(ContextT const& ctx, TokenT& token, bool& skipped_newline)
    { return false; }


    ///////////////////////////////////////////////////////////////////////////
    //
    //  The function 'found_warning_directive' will be called by the library
    //  whenever a #warning directive is found.
    //
    //  The parameter 'ctx' is a reference to the context object used for 
    //  instantiating the preprocessing iterators by the user.
    //
    //  The parameter 'message' references the argument token sequence of the
    //  encountered #warning directive.
    //
    //  If the return value is false, the library throws a preprocessor 
    //  exception of the type 'warning_directive', if the return value is true
    //  the execution continues as if no #warning directive has been found.
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename ContextT, typename ContainerT>
    bool
    found_warning_directive(ContextT const& ctx, ContainerT const& message)
    { return false; }


    ///////////////////////////////////////////////////////////////////////////
    //
    //  The function 'found_error_directive' will be called by the library
    //  whenever a #error directive is found.
    //
    //  The parameter 'ctx' is a reference to the context object used for 
    //  instantiating the preprocessing iterators by the user.
    //
    //  The parameter 'message' references the argument token sequence of the
    //  encountered #error directive.
    //
    //  If the return value is false, the library throws a preprocessor 
    //  exception of the type 'error_directive', if the return value is true
    //  the execution continues as if no #error directive has been found.
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename ContextT, typename ContainerT>
    bool
    found_error_directive(ContextT const& ctx, ContainerT const& message)
    { return false; }

    ///////////////////////////////////////////////////////////////////////////
    //
    //  The function 'found_line_directive' will be called by the library
    //  whenever a #line directive is found.
    //
    //  The parameter 'ctx' is a reference to the context object used for 
    //  instantiating the preprocessing iterators by the user.
    //
    //  The parameter 'arguments' references the argument token sequence of the
    //  encountered #line directive.
    //
    //  The parameter 'line' contains the recognized line number from the #line
    //  directive.
    //
    //  The parameter 'filename' references the recognized file name from the 
    //  #line directive (if there was one given).
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename ContextT, typename ContainerT>
    void
    found_line_directive(ContextT const& ctx, ContainerT const& arguments,
        unsigned int line, std::string const& filename)
    {}

    ///////////////////////////////////////////////////////////////////////////
    //
    //  The function 'throw_exception' will be called by the library whenever a
    //  preprocessing exception occurs.
    //
    //  The parameter 'ctx' is a reference to the context object used for 
    //  instantiating the preprocessing iterators by the user.
    //
    //  The parameter 'e' is the exception object containing detailed error 
    //  information.
    //
    //  The default behavior is to call the function boost::throw_exception.
    //  
    ///////////////////////////////////////////////////////////////////////////
    template <typename ContextT, typename ExceptionT>
    void
    throw_exception(ContextT const& ctx, ExceptionT const& e)
    {
		callback->LogException(e.get_severity(), e.get_errorcode(), e.description());
    }
};

#endif // !defined(BOOST_WAVE_ADVANCED_PREPROCESSING_HOOKS_INCLUDED)