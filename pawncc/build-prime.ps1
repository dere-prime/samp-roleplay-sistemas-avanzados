$PawnCC = Join-Path $PSScriptRoot "pawncc.exe"
$BuildFile = Join-Path $PSScriptRoot "..\scriptfiles\SERVER\build.txt"

& $PawnCC `
    "prime.pwn" `
    "--%" `
    "-Dgamemodes" `
    "-;+" `
    "-(+" `
    "-d2"

if ($LASTEXITCODE -eq 0) {
    $buildDir = Split-Path $BuildFile
    if (!(Test-Path $buildDir)) {
        New-Item -ItemType Directory -Path $buildDir | Out-Null
    }

    if (Test-Path $BuildFile) {
        $count = [int](Get-Content $BuildFile -Raw)
    } else {
        $count = 0
    }

    $count++
    $count | Set-Content $BuildFile

    Write-Host "Build Number: #$count"
}
