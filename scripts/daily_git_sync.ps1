$ErrorActionPreference = "Stop"

$repoRoot = Split-Path -Parent $PSScriptRoot
$logDir = Join-Path $repoRoot ".git\auto-sync-logs"
if (-not (Test-Path $logDir)) {
    New-Item -ItemType Directory -Path $logDir | Out-Null
}

$logFile = Join-Path $logDir ("daily-sync-" + (Get-Date -Format "yyyyMMdd") + ".log")

function Write-Log {
    param([string]$Message)
    $line = "[{0}] {1}" -f (Get-Date -Format "yyyy-MM-dd HH:mm:ss"), $Message
    Add-Content -Path $logFile -Value $line
}

Set-Location $repoRoot
Write-Log "Start daily git sync"

$inside = git rev-parse --is-inside-work-tree 2>$null
if ($inside -ne "true") {
    Write-Log "Not a git repository, abort"
    exit 1
}

$branch = (git branch --show-current).Trim()
if ([string]::IsNullOrWhiteSpace($branch)) {
    Write-Log "Cannot determine current branch, abort"
    exit 1
}

$status = git status --porcelain --untracked-files=normal
if ([string]::IsNullOrWhiteSpace(($status | Out-String).Trim())) {
    Write-Log "No local changes, nothing to commit"
    exit 0
}

git add -A
$staged = git diff --cached --name-only
if ([string]::IsNullOrWhiteSpace(($staged | Out-String).Trim())) {
    Write-Log "No staged changes after git add, nothing to commit"
    exit 0
}

$commitMessage = "chore: daily auto sync " + (Get-Date -Format "yyyy-MM-dd")
git commit -m $commitMessage | Out-Null
Write-Log ("Committed changes with message: " + $commitMessage)

git fetch origin $branch | Out-Null
$local = (git rev-parse HEAD).Trim()
$remote = (git rev-parse ("origin/" + $branch)).Trim()
$base = (git merge-base HEAD ("origin/" + $branch)).Trim()

if ($local -eq $remote) {
    Write-Log "Local equals remote after commit, nothing to push"
    exit 0
}

if ($base -ne $remote) {
    Write-Log "Remote has new commits, skip auto push to avoid conflict"
    exit 2
}

git push origin $branch | Out-Null
Write-Log ("Push completed to origin/" + $branch)
exit 0
