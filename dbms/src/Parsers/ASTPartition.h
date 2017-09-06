#pragma once

#include <Parsers/IAST.h>
#include <common/StringRef.h>


namespace DB
{

/// Either a (possibly compound) literal representing partition value or a partition ID.
class ASTPartition : public IAST
{
public:
    ASTPtr value;
    StringRefs value_fields;

    String id;

    ASTPartition() = default;
    ASTPartition(StringRange range_) : IAST(range_) {}
    String getID() const override;
    ASTPtr clone() const override;

    void setValue(const ASTPtr & value_);

protected:
    void formatImpl(const FormatSettings & settings, FormatState & state, FormatStateStacked frame) const override;
};

}
