inline u32
SafeTruncateToUInt32(u64 value)
{
    Assert(value <= U64Maximum);
    u32 result = (u32)value;

    return result;
}
