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
 * Note that this implementation is obviously overkill for this
 * simple input, but was done rather as an exercise.
 */
#include <stdexcept>

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/variant.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace x3 = boost::spirit::x3;

// Abstract Syntax Tree definition: a structure for our complete input
namespace ast {

struct RuleA { };

struct RuleB { };

struct RuleU {
    using value_type = bool;
    std::vector<value_type> bits;
};

struct RuleR {
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


struct Size {
    int width;
    int generations;
};


struct Init {
    std::vector<int> cells;
};


struct Input {
    Rule rule;
    Size size;
    Init init;
};

}  // namespace ast


// Tell fusion about our structs, this allows the parser to access
// the structs like an indexable array essentially
BOOST_FUSION_ADAPT_STRUCT(ast::RuleA)
BOOST_FUSION_ADAPT_STRUCT(ast::RuleB)
BOOST_FUSION_ADAPT_STRUCT(ast::RuleU, bits)
BOOST_FUSION_ADAPT_STRUCT(ast::RuleR, number)
BOOST_FUSION_ADAPT_STRUCT(ast::Size, width, generations)
BOOST_FUSION_ADAPT_STRUCT(ast::Init, cells)
BOOST_FUSION_ADAPT_STRUCT(ast::Input, rule, size, init)


// Parser grammar
namespace grammar {
    using x3::int_;
    using x3::bool_;
    using x3::int_parser;
    using x3::omit;
    using x3::repeat;

    static x3::rule<class RuleA, ast::RuleA> RuleA = "RuleA";
    static x3::rule<class RuleB, ast::RuleB> RuleB = "RuleB";
    static x3::rule<class RuleU, ast::RuleU> RuleU = "RuleU";
    static x3::rule<class RuleR, ast::RuleR> RuleR = "RuleR";
    static x3::rule<class Rule, ast::Rule> Rule = "Rule";
    static x3::rule<class Size, ast::Size> Size = "Size";
    static x3::rule<class Init, ast::Init> Init = "Init";

    static auto const bit_ = int_parser<bool, 2, 1, 1>();
    static auto const uint8_t_ = int_parser<unsigned char, 10, 1, 3>();
    static auto const RuleA_def = omit['A'];
    static auto const RuleB_def = omit['B'];
    static auto const RuleU_def = 'U' >> repeat(8)[bit_ | bool_];
    static auto const RuleR_def = 'R' >> uint8_t_;
    static auto const Rule_def = RuleA | RuleB | RuleU | RuleR;
    static auto const Size_def = int_ >> int_;
    static auto const Init_def = "init_start" >> *int_ >> "init_end";
    static auto const Input = Rule >> Size >> Init;
    BOOST_SPIRIT_DEFINE(RuleA, RuleB, RuleU, RuleR, Rule, Size, Init);
}  // namespace grammar



using Input = ast::Input;

/**
 * Parse input string into Input structure
 */
Input parse_input_string(const std::string& input_string) {
    using x3::ascii::space;
    using x3::phrase_parse;

    Input result;
    auto iter = input_string.begin();
    auto const end = input_string.end();
    auto const parser = grammar::Input;

    bool r = phrase_parse(iter, end, parser, space, result);
    if (!r || iter != end) {
        throw std::runtime_error("Parse error");
    }
    result.rule.set_number();
    return result;
}

// vim: autoindent syntax=cpp expandtab tabstop=4 softtabstop=4 shiftwidth=4 filetype=cpp
