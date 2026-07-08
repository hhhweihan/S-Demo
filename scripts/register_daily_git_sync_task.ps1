param(
    [string]$TaskName = "S-Demo-Daily-GitSync",
    [string]$StartTime = "21:30"
)

$ErrorActionPreference = "Stop"

$scriptPath = Join-Path $PSScriptRoot "daily_git_sync.ps1"
if (-not (Test-Path $scriptPath)) {
    throw "Cannot find script: $scriptPath"
}

$escapedScriptPath = $scriptPath.Replace('"', '""')
$taskAction = "powershell.exe -NoProfile -ExecutionPolicy Bypass -File `"$escapedScriptPath`""

schtasks /Create /TN $TaskName /SC DAILY /ST $StartTime /TR $taskAction /F | Out-Null
Write-Output "Task registered: $TaskName at $StartTime"
Write-Output "Run now: schtasks /Run /TN $TaskName"
Write-Output "Query: schtasks /Query /TN $TaskName /V /FO LIST"
