inline u32
SafeTruncateToUInt32(u64 value)
{
    Assert(value <= U32Maximum);
    u32 result = (u32)value;

    return result;
}
