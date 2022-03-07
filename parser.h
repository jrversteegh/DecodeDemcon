/**
 * Parse input. The input can be described by the following grammar:
 *
 * input ::= rule space size space init
 * rule ::= 'A' | 'B' | 'U' 8( space bit ) | 'R' space digits
 * size ::= digits space digits
 * init ::= "init_start" *(space digits) space "init_end"
 * space ::= +( ' ' | \t | \n | \r )
 * bit := '0' | '1'
 * digits ::= +digit
 * digit := '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
 *
 * The "R" rule definition is an extension to the syntax specified by
 * the task. The 0-255 integer following it defines the rule number,
 * which is just the decimal representation of the number defined by
 * the 8 bits in the "U" rule specification and was added for
 * convencience.
 *
 * This grammar is being parsed using Spirit X3 using roughly a five
 * stage process.
 * - Create an AST: define the structure(s) of the input.
 * - Fuse the AST to make it accessible to the parser. This is done
 *   with Boost::Fusion. It essentially creates array like
 *   interfaces for the AST structs.
 * - Link syntactic elements to parts of the AST.
 * - Define the expected occurence of syntactic elements in the
 *   input: the grammar.
 * - Do the parse! ..with a provided error handler, so we can tell
 *   the user what the parser potentially didn't like about their
 *   input.
 *
 * Note that this implementation is obviously overkill for this
 * simple input, but was done rather as an exercise.
 */
#include <stdexcept>
#include <sstream>

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/numeric/uint.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>
#include <boost/variant.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#define FMT_HEADER_ONLY
#include <fmt/format.h>

namespace x3 = boost::spirit::x3;

// Abstract Syntax Tree definition: a structure for our complete input
namespace ast {

struct RuleA: x3::position_tagged {};

struct RuleB: x3::position_tagged {};

struct RuleU: x3::position_tagged {
    using value_type = bool;
    std::vector<value_type> bits;
    bool empty() const {
        return bits.empty();
    }
};

struct RuleR: x3::position_tagged {
    int number;
};

struct Rule: x3::variant<RuleA, RuleB, RuleU, RuleR> {
    using base_type::base_type;
    using base_type::operator=;

    int number = -1;

    void set_number() {
        boost::apply_visitor(*this, *this);
    }

    void operator()(const RuleA& rule_a) {
        number = 122;
    }

    void operator()(const RuleB& rule_b) {
        number = 86;
    }

    void operator()(const RuleU& rule_u) {
        number = 0;
        int i = 0;
        for (auto bit: rule_u.bits) {
            number |= bit << i++;
        }
    }

    void operator()(const RuleR& rule_r) {
        number = rule_r.number;
    }
};


struct Size: x3::position_tagged {
    int width;
    int generations;
};


struct Init: x3::position_tagged {
    std::vector<int> cells;
};


struct Input: x3::position_tagged {
    Rule rule;
    Size size;
    Init init;
};

}  // namespace ast


// Tell fusion about our structs, this allows the parser to access
// the structs like a tuple
BOOST_FUSION_ADAPT_STRUCT(ast::RuleA)
BOOST_FUSION_ADAPT_STRUCT(ast::RuleB)
BOOST_FUSION_ADAPT_STRUCT(ast::RuleU, bits)
BOOST_FUSION_ADAPT_STRUCT(ast::RuleR, number)
BOOST_FUSION_ADAPT_STRUCT(ast::Size, width, generations)
BOOST_FUSION_ADAPT_STRUCT(ast::Init, cells)
BOOST_FUSION_ADAPT_STRUCT(ast::Input, rule, size, init)


namespace parser {

struct Error_handler
{
    template <typename Iterator, typename Exception, typename Context>
    x3::error_handler_result on_error(
        Iterator& first, Iterator const& last
      , Exception const& x, Context const& context)
    {
        auto& error_handler = x3::get<x3::error_handler_tag>(context).get();
        std::string message = "Error. Expecting <" + x.which() + "> here:";
        error_handler(x.where(), message);
        return x3::error_handler_result::fail;
    }
};

using x3::int_;
using x3::bool_;
using x3::ascii::space;
using x3::int_parser;
using x3::uint_parser;
using x3::omit;
using x3::repeat;
using x3::expect;
using x3::no_skip;
using x3::lexeme;


struct Bit_class;
struct Byte_class;
struct RuleA_class;
struct RuleB_class;
struct RuleU_class;
struct RuleR_class;
struct Rule_class;
struct Size_class;
struct Init_class;
struct Input_class;

// Link syntactic elements to AST structs with "rules"

x3::rule<class Pos, int> const Pos = "positive int: 1-maxint";
x3::rule<class Space> const Space = "white space";
x3::rule<Bit_class, bool> const Bit = "bit: 0 | 1 | true | false";
x3::rule<Byte_class, unsigned char> const Byte = "byte: 0-255";
x3::rule<RuleA_class, ast::RuleA> const RuleA = "rule A: A";
x3::rule<RuleB_class, ast::RuleB> const RuleB = "rule B: B";
x3::rule<RuleU_class, ast::RuleU> const RuleU = "rule U: U 8(bit) space";
x3::rule<RuleR_class, ast::RuleR> const RuleR = "rule R: R byte";
x3::rule<Rule_class, ast::Rule> const Rule = "rule: A | B | U | R";
x3::rule<Size_class, ast::Size> const Size = "size: int int";
x3::rule<Init_class, ast::Init> const Init = "init: init_start *int init_end";
x3::rule<Input_class, ast::Input> const Input = "input: rule size init";

struct Bit_class: x3::annotate_on_success {};
struct Byte_class: x3::annotate_on_success {};
struct RuleA_class: x3::annotate_on_success {};
struct RuleB_class: x3::annotate_on_success {};
struct RuleU_class: x3::annotate_on_success {};
struct RuleR_class: x3::annotate_on_success {};
struct Rule_class: x3::annotate_on_success {};
struct Size_class: x3::annotate_on_success {};
struct Init_class: x3::annotate_on_success {};
struct Input_class: Error_handler, x3::annotate_on_success {};

// Define parser grammar!

static auto const Pos_def = uint_parser<unsigned, 10>();
static auto const Space_def = no_skip[omit[+space]];
static auto const Bit_def = int_parser<bool, 2, 1, 1>() | bool_;
static auto const Byte_def = uint_parser<unsigned char, 10>();
static auto const RuleA_def = omit['A'];
static auto const RuleB_def = omit['B'];
static auto const RuleU_def = omit['U'] > repeat(8)[expect[Bit]] > Space;
static auto const RuleR_def = omit['R'] > Byte;
static auto const Rule_def = RuleA | RuleB | RuleU | RuleR;
static auto const Size_def = Pos > Pos;
static auto const Init_def = "init_start" > *int_ > "init_end";
static auto const Input_def = expect[Rule] > Size > Init;
BOOST_SPIRIT_DEFINE(Pos, Space, Bit, Byte, RuleA, RuleB, RuleU, RuleR, Rule, Size, Init, Input);

}  // namespace parser


using Input = ast::Input;

/**
 * Parse input string into Input structure
 */
Input parse_input_string(const std::string& input_string) {
    using x3::ascii::space;
    using x3::phrase_parse;
    using x3::with;
    using x3::expect;
    using x3::error_handler_tag;
    using iterator_type = std::string::const_iterator;
    using error_handler_type = x3::error_handler<iterator_type>;

    auto iter = input_string.begin();
    auto const end = input_string.end();
    std::stringstream error_message;
    error_handler_type error_handler(iter, end, error_message);
    auto const parser =
        with<error_handler_tag>(std::ref(error_handler)) [
            expect[parser::Input]
        ];

    Input result;
    try {
        bool r = phrase_parse(iter, end, parser, space, result);
        if (!r || iter != end) {
            // This shouldn't happen: all parser error should be expectation errors
            throw std::runtime_error("Error parsing.");
        }
    }
    catch (x3::expectation_failure<iterator_type>& e) {
        throw std::runtime_error(error_message.str());
    }
    result.rule.set_number();
    return result;
}

// vim: autoindent syntax=cpp expandtab tabstop=4 softtabstop=4 shiftwidth=4 filetype=cpp
