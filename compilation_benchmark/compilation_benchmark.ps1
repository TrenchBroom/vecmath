function benchmark($cpp_name) {
    # /P: preprocess to file
    # /I: specify include directory for vecmath
    # /C: preserve comments during preprocessing

    $preprocessed_output = ($cpp_name).Replace(".cpp", ".i")

    CL /C /I ..\include /P /std:c++17 $cpp_name

    $lines = (Get-Content $preprocessed_output | Measure-Object -Line).Lines
    Write-Output "Lines in $preprocessed_output (preprocessed): $lines"

    $stopwatch = [System.Diagnostics.Stopwatch]::StartNew()
    $runs = 10

    for ($i=1; $i -le $runs; $i++) {
        (CL /I ..\include /std:c++17 $cpp_name) 2>&1> $null
    }

    $stopwatch.Stop()
    $timePerRun = $stopwatch.ElapsedMilliseconds / $runs

    Write-Output "Time to compile $cpp_name (average over $runs runs): $timePerRun ms"
}

benchmark "cmath.cpp"
benchmark "vecmath_forward.cpp"
benchmark "vecmath_all.cpp"
