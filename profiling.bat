@IF EXIST "output/windows/x64/Release/DODExample.exe" (
	@ECHO Using 64-bit executable.
	@SET PROFILE_EXE="output/windows/x64/Release/DODExample.exe"
) ELSE IF EXIST "output/windows/Win32/Release/DODExample.exe" (
	@ECHO Using 32-bit executable.
	@SET PROFILE_EXE="output/windows/Win32/Release/DODExample.exe"
) ELSE (
	@ECHO Missing executable, compile it from source!
	@EXIT /B 0
)

@%PROFILE_EXE% -profiling 100 -ood -scene small
@%PROFILE_EXE% -profiling 100 -dod -scene small
@%PROFILE_EXE% -profiling 100 -ood -scene medium
@%PROFILE_EXE% -profiling 100 -dod -scene medium
@%PROFILE_EXE% -profiling 100 -ood -scene large
@%PROFILE_EXE% -profiling 100 -dod -scene large