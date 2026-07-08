# scripts

## 这个目录是干什么的

这里存放仓库维护过程中使用的辅助脚本，当前主要是日常 Git 同步和定时任务注册相关的 PowerShell 脚本。

这里的东西不多，主要是我自己日常会用到时顺手维护一下。

## 从哪里开始看

- `daily_git_sync.ps1`：执行日常同步逻辑
- `register_daily_git_sync_task.ps1`：注册 Windows 计划任务

这些脚本就是直接跑的 PowerShell 脚本，不过执行前还是要先确认本机的执行策略、Git 环境和任务计划配置。
