# Logging

## Overview
```cpp
#include <Onyx/Utility/Logger.h>
```

Onyx provides a Cross-Platform Logging utility. 
- Thread Safe
- Coloured Output
- Severity Mask 

Note: On Android, the Log output will be displayed via LogCat. 

## Log Severity 
Output verbosity can be controlled via `Onyx::Utility::Log::SetSeverityFlags()`. 
Passing in a combination of `ELogSeverityFlags` (via the OR `|` operator) allows for certain types of output to be masked.

| ELogSeverityFlags | Description | 
| - | - | 
| `NONE` | No Logged messages will be output. |
| `GENERAL` |  Enables `Print` statements. |
| `DEBUG` | Enables `Debug` Statements. |
| `STATUS` | Enables `Message` Statements. |
| `WARNINGS` | Enables `Warning` Statements. | 
| `ERRORS` | Enables `Error` and `Fatal` Errors. | 
| `VALIDATION` | Enabled Validation messages. | 
| `ALL` | Enables All Log output. | 

## Using the Logger
The Logger functions use [printf formatting](https://alvinalexander.com/programming/printf-format-cheat-sheet/).
It is the Caller's responsibility to control newlines, through `\n`. 

```cpp
Onyx::Utility::Log::Print("This is a Print Message!\n");
Onyx::Utility::Log::Print(Onyx::Utility::ELogColour::BLUE, "This is a Print Message in Blue!\n");
Onyx::Utility::Log::Debug("This is a Debug Message!\n");
Onyx::Utility::Log::Message("This is an Info Message!\n");
Onyx::Utility::Log::Validation("Testing", "This is a Validation Message!\n");
Onyx::Utility::Log::Warning("This is a Warning!\n");
Onyx::Utility::Log::Error(__FILE__, __LINE__, __PRETTY_FUNCTION__, "This is an Error Message!\n");
Onyx::Utility::Log::Fatal(__FILE__, __LINE__, __PRETTY_FUNCTION__, "This is a Fatal Error Message!\n");
```
