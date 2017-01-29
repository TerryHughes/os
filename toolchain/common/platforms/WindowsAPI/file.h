typedef enum
{
    ReturnCode_InvalidUsage = 1,

    ReturnCode_Undetermined = 0xFFFFFFFF,
} ReturnCodes;

typedef enum
{
    FileOperationStatus_Success = 0,

    FileOperationStatus_FailedToAcquireFileHandle = 11,
    FileOperationStatus_FailedToGetFileSizeInformation = 12,
    FileOperationStatus_FailedToAcquireMemory = 13,

    FileOperationStatus_UnexpectedNumberOfBytesRead = 21,
    FileOperationStatus_FailedToRead = 22,
} FileOperationStatus;

typedef struct
{
    int status;
    u32 contentSize;
    void *contents;
} ReadFileResult;