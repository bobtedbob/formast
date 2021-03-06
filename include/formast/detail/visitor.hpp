#ifndef FORMAST_DETAIL_VISITOR_HPP_INCLUDED
#define FORMAST_DETAIL_VISITOR_HPP_INCLUDED

// this header file provides an implementation of the AST visitor

#include <cassert>
#include <boost/variant/apply_visitor.hpp>
#include <stdexcept>

#include "ast.hpp"

namespace formast
{
namespace detail
{
namespace visitor
{

template<typename type, typename Visitor>
struct VisitorImpl {
    typedef type result_type;

    VisitorImpl(Visitor & visitor) : visitor(visitor) {};

    result_type expr(formast::detail::ast::Expr const & e) {
        assert(e != 0);
        return boost::apply_visitor(*this, *e);
    }

    result_type operator()(boost::uint64_t const& n) {
        return visitor.uint(n);
    }

    result_type operator()(std::string const& i) {
        return visitor.id(i);
    }

    result_type operator()(const formast::detail::ast::unary_op & x) {
        switch (x.op) {
        case '-':
            return visitor.neg(x.right);
        case '+':
            return visitor.pos(x.right);
        default:
            throw std::runtime_error("unknown unary operator");
        }
    }

    result_type operator()(const formast::detail::ast::binary_op & x) {
        switch (x.op) {
        case '+':
            return visitor.add(x.left, x.right);
        case '-':
            return visitor.sub(x.left, x.right);
        case '*':
            return visitor.mul(x.left, x.right);
        case '/':
            return visitor.div(x.left, x.right);
        default:
            throw std::runtime_error("unknown binary operator");
        }
    }

    Visitor & visitor;

};

}
}
}

#endif
