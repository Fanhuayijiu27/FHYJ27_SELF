# tools/request_vm_build.ps1
# 编码：UTF-8
# 主机端请求虚拟机编译的脚本

$HostSource = "D:\VMwareSHARE\HRproject\31project\_Source\FeatureSpy260722"
$SharedRoot = "D:\VMwareSHARE\HRproject\31project"
$TimeoutSeconds = 60

$ErrorActionPreference = "Stop"
[Console]::OutputEncoding = [System.Text.Encoding]::UTF8

$SharedSourceDir = Join-Path $SharedRoot "shared_source_autoVS2015\FeatureSpy260722"
$BuildBridgeDir = Join-Path $SharedRoot "build_bridge_autoVS2015"

if (-not (Test-Path $SharedSourceDir)) {
    New-Item -ItemType Directory -Path $SharedSourceDir -Force | Out-Null
    Write-Host "创建共享源码目录: $SharedSourceDir "
}
if (-not (Test-Path $BuildBridgeDir)) {
    New-Item -ItemType Directory -Path $BuildBridgeDir -Force | Out-Null
    Write-Host "创建编译桥接目录: $BuildBridgeDir "
}

$StatusRequestTmp = Join-Path $BuildBridgeDir "build.request.tmp"
$StatusRequest    = Join-Path $BuildBridgeDir "build.request"
$StatusRunning    = Join-Path $BuildBridgeDir "build.running"
$StatusDone       = Join-Path $BuildBridgeDir "build.done"
$StatusFailed     = Join-Path $BuildBridgeDir "build.failed"
$LogFile          = Join-Path $BuildBridgeDir "build_latest.log"

if (Test-Path $StatusRunning) {
    Write-Host "发现 build.running，虚拟机正在编译中，请勿重复提交任务！ " -ForegroundColor Red
    exit 1
}

Write-Host "清理旧的编译状态文件... "
Remove-Item -Path $StatusRequestTmp -ErrorAction SilentlyContinue
Remove-Item -Path $StatusRequest -ErrorAction SilentlyContinue
Remove-Item -Path $StatusDone -ErrorAction SilentlyContinue
Remove-Item -Path $StatusFailed -ErrorAction SilentlyContinue
Remove-Item -Path $LogFile -ErrorAction SilentlyContinue

Write-Host "开始同步源码到共享目录: $SharedSourceDir "
$robocopyArgs = @(
    $HostSource,
    $SharedSourceDir,
    "/MIR",
    "/XD", ".git", ".vs", "x64", "Debug", "Release",
    "/XF", "*.sdf", "*.opensdf", "*.user", "*.log",
    "/NJH", "/NJS", "/NDL", "/NC", "/NS"
)
& robocopy @robocopyArgs
if ($LASTEXITCODE -ge 8) {
    Write-Host "Robocopy 源码同步失败，退出码：$LASTEXITCODE " -ForegroundColor Red
    exit 1
}
Write-Host "源码同步完成。 "

$BuildId = Get-Date -Format "yyyyMMdd_HHmmss"
Write-Host "生成编译请求: $BuildId "

Set-Content -Path $StatusRequestTmp -Value $BuildId -Encoding UTF8
Rename-Item -Path $StatusRequestTmp -NewName "build.request" -Force

Write-Host "等待虚拟机处理编译请求... "
$StartTime = Get-Date
$IsFinished = $false
$ExitCode = 0

while (($IsFinished -eq $false) -and (((Get-Date) - $StartTime).TotalSeconds -lt $TimeoutSeconds)) {
    Start-Sleep -Seconds 2

    if (Test-Path $StatusDone) {
        Write-Host "
编译成功！ (build.done) " -ForegroundColor Green
        $IsFinished = $true
        $ExitCode = 0
    }
    elseif (Test-Path $StatusFailed) {
        Write-Host "
编译失败！ (build.failed) " -ForegroundColor Red
        $IsFinished = $true
        $ExitCode = 1
    }
    else {
        Write-Host "." -NoNewline
    }
}

Write-Host "
"

if (-not $IsFinished) {
    Write-Host "编译请求超时 ($TimeoutSeconds 秒)！ " -ForegroundColor Red
    $ExitCode = 2
}

if (Test-Path $LogFile) {
    Write-Host "================ 编译日志 build_latest.log ================ " -ForegroundColor Cyan
    Get-Content $LogFile -Encoding Default | Write-Host
    Write-Host "=========================================================== " -ForegroundColor Cyan
} else {
    Write-Host "未找到编译日志: $LogFile " -ForegroundColor Yellow
}

exit $ExitCode