在 Windows 上通过 Git Subtree Fork GitHub 子目录（保留历史并可提交）
我将提供一个完整的 Windows 解决方案，通过 Git Subtree 在您的项目中 fork 别人的 GitHub 仓库子目录，保留完整历史记录，并能独立提交到自己的仓库。
📦 环境准备
1. 安装必要工具
# 1. 确保已安装 Git for Windows
git --version

# 2. 如果需要图形界面，安装 Git GUI
# 从 https://git-scm.com/download/win 下载

# 3. 安装 Visual Studio Code（可选但推荐）
# 用于更好的代码查看和编辑
2. 配置 Git
# 设置用户名和邮箱
git config --global user.name "你的名字"
git config --global user.email "你的邮箱"

# 设置默认文本编辑器为 VS Code
git config --global core.editor "code --wait"

# 设置行尾转换（Windows 推荐）
git config --global core.autocrlf true
🚀 完整操作流程
方案1：使用 Git Subtree（推荐）
步骤1：准备您的项目
# 打开 PowerShell 或 Git Bash
# 导航到您的项目目录
cd D:\MyProjects\your-repo

# 确保您的项目是 git 仓库
git init
步骤2：添加原始仓库为远程仓库
# 添加要 fork 的仓库作为远程源
# 格式：git remote add <名称> <仓库URL>
git remote add upstream https://github.com/otheruser/other-repo.git

# 验证远程仓库
git remote -v
# 应该看到：
# origin    https://github.com/yourname/your-repo.git (fetch)
# origin    https://github.com/yourname/your-repo.git (push)
# upstream  https://github.com/otheruser/other-repo.git (fetch)
# upstream  https://github.com/otheruser/other-repo.git (push)
步骤3：获取原始仓库数据
# 获取上游仓库的所有分支和提交
git fetch upstream

# 查看上游仓库的分支
git branch -r | grep upstream
步骤4：使用 subtree 添加子目录
# 假设您要 fork 上游仓库的 "src/components" 目录
# 到您的项目的 "third_party/components" 目录

# 添加子树（不压缩历史）
git subtree add --prefix=third_party/components upstream/main --squash

# 或者，如果您想保留完整历史（推荐）
git subtree add --prefix=third_party/components upstream/main
说明：
--prefix=third_party/components：指定在您项目中的目标路径
upstream/main：指定远程仓库和分支
--squash：将原始仓库的所有相关提交合并为一次提交（可选）
步骤5：验证历史记录
# 查看提交历史
git log --oneline --graph

# 查看特定文件的提交历史
git log --oneline -- third_party/components/README.md

# 查看目录树
git ls-tree HEAD third_party/components