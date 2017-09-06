#include <Parsers/ASTPartition.h>
#include <Parsers/ASTFunction.h>
#include <IO/WriteHelpers.h>
#include <Common/typeid_cast.h>

namespace DB
{

String ASTPartition::getID() const
{
    if (value)
        return "Partition";
    else
        return "Partition_ID_" + id;
}

ASTPtr ASTPartition::clone() const
{
    auto res = std::make_shared<ASTPartition>(*this);
    res->children.clear();

    if (value)
    {
        res->value = value->clone();
        res->children.push_back(res->value);
    }

    return res;
}

void ASTPartition::setValue(const ASTPtr & value_)
{
    value = value_;
    children.push_back(value);

    const auto * tuple_lit = typeid_cast<const ASTFunction *>(value.get());
    if (tuple_lit && tuple_lit->name == "tuple")
    {
        for (const ASTPtr & field : tuple_lit->arguments->children)
            value_fields.emplace_back(field->range.first, field->range.second - field->range.first);
    }
    else
        value_fields.emplace_back(value->range.first, value->range.second - value->range.first);
}

void ASTPartition::formatImpl(const FormatSettings & settings, FormatState & state, FormatStateStacked frame) const
{
    if (value)
    {
        value->formatImpl(settings, state, frame);
    }
    else
    {
        settings.ostr << (settings.hilite ? hilite_keyword : "") << "ID " << (settings.hilite ? hilite_none : "");
        WriteBufferFromOwnString id_buf;
        writeQuoted(id, id_buf);
        settings.ostr << id_buf.str();
    }
}

}
