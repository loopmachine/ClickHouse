#include <Parsers/ParserPartition.h>
#include <Parsers/CommonParsers.h>
#include <Parsers/ExpressionElementParsers.h>
#include <Parsers/ASTPartition.h>
#include <Parsers/ASTLiteral.h>

namespace DB
{

bool ParserPartition::parseImpl(Pos & pos, ASTPtr & node, Expected & expected)
{
    ParserKeyword s_id("ID");
    ParserStringLiteral parser_string_literal;
    ParserAnyLiteral parser_any_literal;

    Pos begin = pos;

    auto partition = std::make_shared<ASTPartition>();

    if (s_id.ignore(pos, expected))
    {
        ASTPtr partition_id;
        if (!parser_string_literal.parse(pos, partition_id, expected))
            return false;

        partition->id = dynamic_cast<const ASTLiteral &>(*partition_id).value.get<String>();
    }
    else
    {
        ASTPtr value;
        if (!parser_any_literal.parse(pos, value, expected))
            return false;

        partition->setValue(value);
    }

    partition->range = StringRange(begin, pos);
    node = partition;
    return true;
}

}
