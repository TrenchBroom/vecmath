# /P: preprocess to file
# /I: specify include directory for vecmath
# /C: preserve comments during preprocessing

CL /C /I ..\include /P /std:c++17 compilation_benchmark.cpp

$lines = (Get-Content compilation_benchmark.i | Measure-Object -Line).Lines
Write-Output "Lines (preprocessed): $lines"

$stopwatch = [System.Diagnostics.Stopwatch]::StartNew()
$runs = 10

for ($i=1; $i -le $runs; $i++) {
    (CL /I ..\include /std:c++17 compilation_benchmark.cpp) 2>&1> $null
}

$stopwatch.Stop()
$timePerRun = $stopwatch.ElapsedMilliseconds / $runs

Write-Output "Time to compile (average over $runs runs): $timePerRun ms"