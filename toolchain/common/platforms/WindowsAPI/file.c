internal ReadFileResult
WindowsAPI_ReadFile(char *filePath)
{
    ReadFileResult result = {0};

    HANDLE fileHandle = CreateFileA(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        PrintLine("Failed to acquire file handle to '%s'", filePath);

        result.status = FileOperationStatus_FailedToAcquireFileHandle;
    }
    else
    {
        LARGE_INTEGER fileSize;
        if (GetFileSizeEx(fileHandle, &fileSize))
        {
            result.contentSize = SafeTruncateToUInt32(fileSize.QuadPart);
            result.contents = VirtualAlloc(NULL, result.contentSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
            if (result.contents)
            {
                DWORD bytesRead;
                if (ReadFile(fileHandle, result.contents, result.contentSize, &bytesRead, NULL))
                {
                    if (bytesRead == result.contentSize)
                    {
                        result.status = FileOperationStatus_Success;
                    }
                    else
                    {
                        PrintLine("Expected to read %d byte%s but actually read %d byte%s", result.contentSize, result.contentSize == 1 ? "" : "s", bytesRead, bytesRead == 1 ? "" : "s");

                        VirtualFree(result.contents, 0, MEM_RELEASE);

                        result.status = FileOperationStatus_UnexpectedNumberOfBytesRead;
                    }
                }
                else
                {
                    PrintLine("Failed to read file '%s'", filePath);

                    VirtualFree(result.contents, 0, MEM_RELEASE);

                    result.status = FileOperationStatus_FailedToRead;
                }
            }
            else
            {
                // TODO(TerryH): logging

                result.status = FileOperationStatus_FailedToAcquireMemory;
            }
        }
        else
        {
            // TODO(TerryH): logging

            result.status = FileOperationStatus_FailedToGetFileSizeInformation;
        }

        CloseHandle(fileHandle);
    }

    return result;
}
