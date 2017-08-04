# IsWindowsPlayingSound
A simple friction-free zero-dependencies c++ console application that determines if Windows is playing a sound or not.

You can use it simply from the command line, and it will display a message, or use the standard command line error level. The app will set error level to 0 (no sound playing) or 1 (sound playing).

From PowerShell, you can use the `$lastexitcode` as explained here: https://blogs.msdn.microsoft.com/powershell/2006/09/15/errorlevel-equivalent/
